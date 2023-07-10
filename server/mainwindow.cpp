#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ztcpserver.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->serverWidget->init("push", "127.0.0.1", 8888);
}

MainWindow::~MainWindow()
{
    delete ui;
}
