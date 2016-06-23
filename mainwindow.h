#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QFileSystemModel>
#include <editview.h>
#include "consoleoutput.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString SpecificElabPath;
    QString basePath;

private:
    Ui::MainWindow *ui;

////////////////////////////
    void createActions();
    void createMenus();
    void readXml(const QString &fileName);
    void initDirectories();
    void openProject(const QString &dir);
    void saveDirectory(const QString &dir);
    void loadDirectory();
////////////////////////////
    QMenu *fileMenu;
    QMenu *helpMenu;

    QAction *loadAct;
    QAction *helpAct;

    QFileSystemModel *dirModel;

    EditView *editView;
    ConsoleOutput *consoleOutput;
    QString lastfile;
private slots:
    void loadProject();
    void helpInfo();
    void elaborate(bool clicked);
    void elaborationEnded(int f);

    void on_treeView_clicked(const QModelIndex &index);
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);

};

#endif // MAINWINDOW_H
