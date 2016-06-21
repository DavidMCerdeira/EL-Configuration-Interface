#include "consoleoutput.h"
#include "ui_consoleoutput.h"
#include <QDebug>
#include <QSysInfo>

#define WIND_WIDTH 640
#define WIND_HEIGHT 480

ConsoleOutput::ConsoleOutput(QString path, QWidget *parent) :
    elaborationPath(path),
    QDialog(parent),
    ui(new Ui::ConsoleOutput)
{

#ifdef Q_WS_WIN
    runnable = "runElaborator.bat";
#else
    runnable = "runElaborator.sh";
#endif

    ui->setupUi(this);
    resize(WIND_WIDTH, WIND_HEIGHT);
    elaborate();
}

ConsoleOutput::~ConsoleOutput()
{
    delete ui;
}

void ConsoleOutput::showError(QString str)
{
    //Show Standard Error with red color
    ui->consoleOutput->appendHtml("<p style=\"color:red;\">" + str + "</p>");
    qDebug() << "Error: " + str;
}

void ConsoleOutput::showOutput(QString str)
{
    ui->consoleOutput->appendPlainText(str);
    qDebug() << "Out: " + str;
}

void ConsoleOutput::showErrorList(QStringList strList)
{
    int s = strList.size();
    for(int i = 0; i < s; i++){
        showError(strList[i]);
    }
}

void ConsoleOutput::showOutputList(QStringList strList)
{
    int s = strList.size();
    for(int i = 0; i < s; i++){
        showOutput(strList[i]);
    }
}

void ConsoleOutput::elaborate()
{
    //QString commandElaborate =  "java -jar ./Elaborator.jar";
    QString command = elaborationPath + runnable;

    process = new QProcess();
    connect(process, SIGNAL(readyRead()), this, SLOT(elaborateOutputHandler()));
    connect(process, SIGNAL(finished(int)), this, SLOT(onExit(int)));
    process->start(command);
    qDebug() << command;
}

QStringList ConsoleOutput::getLinesFromQByteArray(QByteArray qba)
{
    int s = qba.size(); //number of characters
    QStringList qsl = QStringList(); //list where lines are stored

    QString temp = QString(""); //temporary variable

    //a line ends with a '\n' or a '\r'
    for(int i = 0; i < s; i++)
    {
        if((qba[i] == '\n') || (qba[i] == '\r')) //found end of line
        {
            if(temp != "")
                qsl.append(temp); //insert line into list
            temp = QString(""); //start a new line
        }
        else{
            //append character to current line
            temp.append(qba.at(i));
        }
    }

    if(temp != "") // if we miss the last line, insert it
        qsl.append(temp);

    return qsl;
}

void ConsoleOutput::elaborateOutputHandler()
{
    QStringList errorList = getLinesFromQByteArray((process->readAllStandardError()));
    showErrorList(errorList);

    //Show Standard Output lines
    QStringList outputList = getLinesFromQByteArray((process->readAllStandardOutput()));
    showOutputList(outputList);
}

void ConsoleOutput::onExit(int err)
{
    QString errorMsg = "Elaborator ended execution with code: " + QString::number(err);

    if(err == 0)
        showOutput(errorMsg);
    else
        showError(errorMsg);
}