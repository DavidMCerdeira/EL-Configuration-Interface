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
#include <QDesktopServices>

#define WIND_WIDTH 450
#define WIND_HEIGHT 120
#define WIND_HEIGHT2 500
#define LAST_FILE "lastproject.rs"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();
    createMenus();

    initDirectories(&dirModel_confs, QStringList("*.xml"), ui->treeView_configs);
    initDirectories(&dirModel_elabs, QStringList("*.java"), ui->treeView_elaborations);

    connect(ui->elaborateButton, SIGNAL(clicked(bool)), this, SLOT(elaborate(bool)));

    ui->elaborateButton->setHidden(true);
    ui->tabWidget->setHidden(true);
    ui->treeView_configs->setVisible(true);
    ui->treeView_configs->setExpandsOnDoubleClick(false);

    ui->openFolder->setEnabled(false);

    setAcceptDrops( true); //Drag & Drop

    setWindowTitle(tr("ELCI (EL Configuration Interface)"));
    setMinimumSize(300, WIND_HEIGHT);
    resize(WIND_WIDTH, WIND_HEIGHT);
    setWindowIcon(QIcon(":/icon.ico"));
    loadDirectory();
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
        ui->tabWidget->setVisible(false);
        ui->elaborateButton->setHidden(true);
        statusBar()->showMessage(message);
        return;
    }

    //======================================= Check if it contains at least one TopLevel
    QDirIterator directories(ConfigPath, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    QString subpath;

    if(!directories.hasNext()){
        ui->tabWidget->setVisible(false);
        ui->elaborateButton->setHidden(true);
        QString message = tr("Project has no folder inside /EL/Configs/");
        statusBar()->showMessage(message);
        return;
    }

    //======================================= Check if it contains >1 TopLevel
    subpath = directories.next(); //next folder
    if(directories.hasNext()){
        ui->treeView_configs->setRootIndex(dirModel_confs->index(ConfigPath));//>1 TopLevel (error?)
    }
    else
        ui->treeView_configs->setRootIndex(dirModel_confs->index(subpath));//Has 1 TopLevel (go inside)

    ui->treeView_elaborations->setRootIndex(dirModel_elabs->index(SpecificElabPath));

    ui->tabWidget->setVisible(true);
    ui->treeView_elaborations->setVisible(true);

    ui->elaborateButton->setHidden(false);
    QString message = tr("Project Loaded Successfully!");
    resize(WIND_WIDTH, WIND_HEIGHT2);
    statusBar()->showMessage(message);
    //ui->treeView->setCurrentIndex(dirModel->index(directoryRoot));
    saveDirectory(dir);
}

void MainWindow::saveDirectory(const QString &dir)
{
    QFile outputFile(LAST_FILE);
    outputFile.open(QIODevice::WriteOnly);

    if(outputFile.isOpen()){
        QTextStream outStream(&outputFile);
        outStream << dir;
        outputFile.close();
    }
}

void MainWindow::loadDirectory()
{
    QFile inputFile(LAST_FILE);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       lastfile = in.readLine();
       inputFile.close();
       if (QDir(lastfile).exists())
           openProject(lastfile);
    }
}



void MainWindow::elaborate(bool clicked)
{
    //qDebug() << "Here";
    ui->elaborateButton->setEnabled(false);
    consoleOutput = new ConsoleOutput(basePath);
    connect(consoleOutput, SIGNAL(finished(int)), this, SLOT(elaborationEnded(int)));
    consoleOutput->show();
}

void MainWindow::helpInfo(){  
    QMessageBox::information(this,"Help","Engineered by the Stack IoT team.");
}

void MainWindow::elaborationEnded(int f)
{
    //delete consoleOutput; //TODO, This causes problems on some machines, why?
    ui->elaborateButton->setEnabled(true);
}

void MainWindow::createActions()
{
    loadAct = new QAction(tr("&Load Project"), this);
    loadAct->setShortcuts(QKeySequence::Open);
    loadAct->setStatusTip(tr("Load EL project directory."));
    connect(loadAct, &QAction::triggered, this, &MainWindow::loadProject);

    helpAct = new QAction(tr("&About"), this);
    helpAct->setShortcuts(QKeySequence::HelpContents);
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

void MainWindow::initDirectories(QFileSystemModel **dirModel, QStringList filter, QTreeView *treeView){
    // Creates a new model and populate
    (*dirModel) = new QFileSystemModel(this);

    // Set filter
    //dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    (*dirModel)->setNameFilters(filter);
    (*dirModel)->setNameFilterDisables(false); //not gray
    (*dirModel)->setReadOnly(true);

    // QFileSystemModel requires root path
    (*dirModel)->setRootPath("");

    // Attach the model to the view
    treeView->setModel(*dirModel);

    treeView->setColumnHidden(1, true);
    treeView->setColumnHidden(2, true);
    treeView->setColumnHidden(3, true);
    treeView->setVisible(false);
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

void expandChildren(const QModelIndex &index, QTreeView *view)
{
    if (!index.isValid()) return;
    int childCount = index.model()->rowCount(index);
    for (int i = 0; i < childCount; i++) {
        const QModelIndex &child = index.child(i, 0);
        expandChildren(child, view);
    }
    if (!view->isExpanded(index)) view->setExpanded(index,true);
}

void MainWindow::on_treeView_configs_clicked(const QModelIndex &index)
{
    QString mPath = dirModel_confs->fileInfo(index).absoluteFilePath();
    QFileInfo file(mPath);

    if( file.exists() && file.isFile()){
        editView = new EditView;
        editView->readXml(mPath,"",SpecificElabPath,EditView::READTYPE::GENERAL);
        editView->show();
    }
    else if( file.exists() && file.isDir()){ //expand children
        ui->treeView_configs->setExpanded(index, true);
        expandChildren(index,ui->treeView_configs);
    }
}

void MainWindow::on_treeView_elaborations_clicked(const QModelIndex &index)
{
    QString mPath = dirModel_elabs->fileInfo(index).absoluteFilePath();
    QFileInfo file(mPath);

    if( file.exists() && file.isFile()){
        QDesktopServices::openUrl(QUrl(mPath));
        ui->openFolder->setEnabled(false);
    } else {
        curElabTree = index;
        ui->openFolder->setEnabled(true);
    }
}

void MainWindow::on_openFolder_clicked()
{
    QString mPath = dirModel_elabs->fileInfo(curElabTree).absoluteFilePath();
    QFileInfo file(mPath);

    if(!file.isFile()){
        qDebug() << "OpenFolder:"<<mPath;

        QDesktopServices::openUrl(QUrl(mPath));
    }
}


