#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QFileSystemModel>
#include <editview.h>
#include "consoleoutput.h"
#include <QTreeView>
#include <QModelIndex>

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
    void openProject(const QString &dir);
    void initDirectories(QFileSystemModel **dirModel, QStringList filter, QTreeView *treeView);
  	void saveDirectory(const QString &dir);
    void loadDirectory();
////////////////////////////
    QMenu *fileMenu;
    QMenu *helpMenu;

    QAction *loadAct;
    QAction *helpAct;

    QFileSystemModel *dirModel_confs;
    QFileSystemModel *dirModel_elabs;

    EditView *editView;
    ConsoleOutput *consoleOutput;
 	QString lastfile;
    QModelIndex curElabTree;
private slots:
    void loadProject();
    void helpInfo();
    void elaborate();
    void elaborationEnded(int f);

    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);

    void on_treeView_configs_clicked(const QModelIndex &index);
    void on_treeView_elaborations_clicked(const QModelIndex &index);

    void on_openFolder_clicked();
};

#endif // MAINWINDOW_H
