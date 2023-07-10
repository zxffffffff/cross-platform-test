#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->spinBox_row, &QSpinBox::textChanged, this, &MainWindow::onRowColChanged);
    connect(ui->spinBox_col, &QSpinBox::textChanged, this, &MainWindow::onRowColChanged);
    onRowColChanged();

    connect(&m_zTcpClient, &ZTcpClient::sgnDebugStr, [this](QString msg)
    {
        QString t = QTime::currentTime().toString("hh:mm:ss.zzz");
        ui->editMsg->appendPlainText(t + " " + msg);
    });
    connect(&m_zTcpClient, &ZTcpClient::sgnOnRead, this, &MainWindow::on_tcp_read);

    ui->btnReq->setEnabled(false);
    ui->btnPush->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onRowColChanged()
{
    int rowCount = ui->spinBox_row->value();
    int columnCount = ui->spinBox_col->value();

    ui->tableWidget->setRowCount(rowCount);
    ui->tableWidget->setColumnCount(columnCount);
    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < columnCount; ++j) {
            auto p = ui->tableWidget->item(i, j);
            if (!p) {
                p = new QTableWidgetItem(tr("%1,%2").arg(i+1).arg(j+1));
                ui->tableWidget->setItem(i, j, p);
            }
        }
    }
}

void MainWindow::on_btnRun_clicked()
{
    if(ui->btnRun->text() == "stop") {
        m_zTcpClient.Stop();

        ui->editAddr->setEnabled(true);
        ui->editPort->setEnabled(true);
        ui->btnRun->setText("run");
        ui->btnReq->setEnabled(false);
        ui->btnPush->setEnabled(false);
        return;
    }

    ui->editMsg->clear();

    QString addr = ui->editAddr->text();
    int port = ui->editPort->text().toInt();
    bool ok = m_zTcpClient.Start(addr, port);
    if (!ok)
        return;

    ui->editAddr->setEnabled(false);
    ui->editPort->setEnabled(false);
    ui->btnRun->setText("stop");
    ui->btnReq->setEnabled(true);
    ui->btnPush->setEnabled(true);
}

void MainWindow::on_btnReq_clicked()
{
    QJsonObject jsonObject;
    jsonObject["api"] = "req";
    jsonObject["row"] = ui->spinBox_row->value();
    jsonObject["col"] = ui->spinBox_col->value();
    QJsonDocument jsonDocument(jsonObject);
    QString jsonString = jsonDocument.toJson(QJsonDocument::Indented);
    m_zTcpClient.Send(jsonString);
}

void MainWindow::on_btnPush_clicked()
{
    QJsonObject jsonObject;
    jsonObject["api"] = "push";
    jsonObject["row"] = ui->spinBox_row->value();
    jsonObject["col"] = ui->spinBox_col->value();
    QJsonDocument jsonDocument(jsonObject);
    QString jsonString = jsonDocument.toJson(QJsonDocument::Indented);
    m_zTcpClient.Send(jsonString);
}

void MainWindow::on_tcp_read(QString buf)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(buf.toUtf8());
    QJsonObject jsonObject = jsonDocument.object();
    QJsonArray jsonArray = jsonObject["data"].toArray();
    const int row = jsonArray.size();
    for (int i = 0; i < row; ++i) {
        QString item = jsonArray.at(i).toString();
        auto list = item.split(' ');
        const int col = list.size();
        for (int j = 0; j < col; ++j) {
            auto p = ui->tableWidget->item(i, j);
            if (p) {
                p->setText(list[j]);
            }
        }
    }
}
