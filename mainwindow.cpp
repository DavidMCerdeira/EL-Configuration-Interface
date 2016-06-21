#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QFile>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDomDocument>
#include <iostream>
#include <QDirModel>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QSplitter>
#include <QDebug>
#define WIND_WIDTH 450
#define WIND_HEIGHT 120
#define WIND_HEIGHT2 500

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();
    createMenus();
    initDirectories();

    connect(ui->elaborateButton, SIGNAL(clicked(bool)), this, SLOT(elaborate(bool)));
    ui->elaborateButton->setHidden(true);

    setAcceptDrops(true); //Drag & Drop


    setWindowTitle(tr("ELCI (EL Configuration Interface)"));
    setMinimumSize(300, WIND_HEIGHT);
    resize(WIND_WIDTH, WIND_HEIGHT);
    setWindowIcon(QIcon(":/icon.ico"));
}

void MainWindow::loadProject(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Project Directory"),
     "",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (dir.isEmpty()) return; //User canceled QFileDialog
    openProject(dir);
}

void MainWindow::openProject(const QString &dir){
    basePath = dir + '/';
    SpecificElabPath = dir + "/EL/SpecificElaborations/";
    QString ConfigPath = dir + "/EL/Configs/";
    resize(WIND_WIDTH, WIND_HEIGHT);

    //======================================= Test Subdirectories
    if(!QDir(SpecificElabPath).exists() || !QDir(ConfigPath).exists()){
        QString message = tr("Project does not contain the directory /EL/SpecificElaborations/ or /EL/Configs/");
        ui->treeView->setVisible(false);
        ui->elaborateButton->setHidden(true);
        ui->label->setVisible(true);
        statusBar()->showMessage(message);
        return;
    }

    //======================================= Check if it contains at least one TopLevel
    QDirIterator directories(ConfigPath, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    QString subpath;

    if(!directories.hasNext()){
        ui->treeView->setVisible(false);
        ui->elaborateButton->setHidden(true);
        ui->label->setVisible(true);
        QString message = tr("Project has no folder inside /EL/Configs/");
        statusBar()->showMessage(message);
        return;
    }

    //======================================= Check if it contains >1 TopLevel
    subpath = directories.next(); //next folder
    if(directories.hasNext())
        ui->treeView->setRootIndex(dirModel->index(ConfigPath));//>1 TopLevel (error?)
    else
        ui->treeView->setRootIndex(dirModel->index(subpath));//Has 1 TopLevel (go inside)

    ui->treeView->setVisible(true);
    ui->elaborateButton->setHidden(false);
    ui->label->setVisible(false);
    QString message = tr("Project Loaded Successfully!");
    resize(WIND_WIDTH, WIND_HEIGHT2);
    statusBar()->showMessage(message);
    //ui->treeView->setCurrentIndex(dirModel->index(directoryRoot));
}

void MainWindow::elaborate(bool clicked)
{
    //qDebug() << "Here";
    ui->elaborateButton->setEnabled(false);
    consoleOutput = new ConsoleOutput(basePath);
    connect(consoleOutput, SIGNAL(accepted()), this, SLOT(elaborationEnded()));
    consoleOutput->show();
}

void MainWindow::helpInfo(){  
    QMessageBox::information(this,"Help","NÃO QUERO SABER! ISSO É PROBLEMA TEU!");
}

void MainWindow::elaborationEnded()
{
    delete consoleOutput;
    ui->elaborateButton->setEnabled(true);
}

void MainWindow::createActions()
{
    loadAct = new QAction(tr("&Load"), this);
    loadAct->setShortcuts(QKeySequence::New);
    loadAct->setStatusTip(tr("Load EL project directory."));
    connect(loadAct, &QAction::triggered, this, &MainWindow::loadProject);

    helpAct = new QAction(tr("&More..."), this);
    helpAct->setShortcuts(QKeySequence::New);
    helpAct->setStatusTip(tr("Load EL project directory."));
    connect(helpAct, &QAction::triggered, this, &MainWindow::helpInfo);
}


void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(loadAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAct);
}

void MainWindow::initDirectories()
{
    /*
    QDirModel *model
    model = new QDirModel(this);// Create and populate our model
    model->setReadOnly(true);// Disable modifying file system
    // Tie TreeView with DirModel
    // QTreeView::setModel(QAbstractItemModel * model)
    // Reimplemented from QAbstractItemView::setModel().
    ui->treeView->setModel(model);
    //ui->treeView->setGeometry(0,0,50,100);*/



    // Creates our new model and populate
    QString mPath = "C:/Users/Miguel/Desktop/";
    dirModel = new QFileSystemModel(this);

    // Set filter
    //dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirModel->setNameFilters(QStringList("*.xml"));
    dirModel->setNameFilterDisables(false); //not gray

    // QFileSystemModel requires root path
    dirModel->setRootPath(mPath);

    // Attach the model to the view
    ui->treeView->setModel(dirModel);

    ui->treeView->setColumnHidden(1,true);
    ui->treeView->setColumnHidden(2,true);
    ui->treeView->setColumnHidden(3,true);
    ui->treeView->setVisible(false);
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    QString mPath = dirModel->fileInfo(index).absoluteFilePath();
    QFileInfo file(mPath);

    if( file.exists() && file.isFile()){
        editView = new EditView;
        editView->readXml(mPath);
        editView->show();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    //foreach (const QUrl &url, e->mimeData()->urls())
    openProject(e->mimeData()->urls().first().toLocalFile());

}

MainWindow::~MainWindow()
{
    delete ui;
}
