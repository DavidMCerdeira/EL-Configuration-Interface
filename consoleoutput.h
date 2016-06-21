#ifndef CONSOLEOUTPUT_H
#define CONSOLEOUTPUT_H

#include <QDialog>
#include <QProcess>

namespace Ui {
class ConsoleOutput;
}

class ConsoleOutput : public QDialog
{
    Q_OBJECT

public:
    explicit ConsoleOutput(QString path, QWidget *parent = 0);
    ~ConsoleOutput();

    QString runnable;

private slots:
    void elaborateOutputHandler();
    void onExit(int err);

private:
    Ui::ConsoleOutput *ui;
    const QString elaborationPath;
    void showError(QString str);
    void showOutput(QString str);
    void showErrorList(QStringList strList);
    void showOutputList(QStringList strList);
    static QStringList getLinesFromQByteArray(QByteArray qba);
    void elaborate();

    QProcess *process;


};

#endif // CONSOLEOUTPUT_H
