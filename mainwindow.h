#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QFileSystemModel>
#include <editview.h>

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

private:
    Ui::MainWindow *ui;

////////////////////////////
    void createActions();
    void createMenus();
    void readXml(const QString &fileName);
    void initDirectories();
    void openProject(const QString &dir);
////////////////////////////
    QMenu *fileMenu;
    QMenu *helpMenu;

    QAction *loadAct;
    QAction *helpAct;

    QFileSystemModel *dirModel;

    EditView *editView;
private slots:
    void loadProject();
    void helpInfo();

    void on_treeView_clicked(const QModelIndex &index);
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);

};

#endif // MAINWINDOW_H
