#ifndef CONSOLEOUTPUT_H
#define CONSOLEOUTPUT_H

#include <QDialog>
#include <QProcess>

#define FINAL_FILES_SUFIX "EL/FinalFiles"

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
    void copySrcs(bool dummy);

    void on_showGeneratedButton_clicked();

signals:
    void close();

private:
    Ui::ConsoleOutput *ui;
    const QString elaborationPath;
    const QString finalFilesPath;
    void showError(QString str);
    void showOutput(QString str);
    void showErrorList(QStringList strList);
    void showOutputList(QStringList strList);
    static QStringList getLinesFromQByteArray(QByteArray qba);
    void elaborate();

    QProcess *process;
    QString lastPath;
};

#endif // CONSOLEOUTPUT_H
