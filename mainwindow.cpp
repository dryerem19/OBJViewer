#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->objectTreeWidget->setScene(ui->sceneWidget->getScene());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_file_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open OBJ"), qApp->applicationDirPath(), tr("OBJ Files (*.obj)"));
    ui->sceneWidget->loadObjFromFile(fileName);
}

