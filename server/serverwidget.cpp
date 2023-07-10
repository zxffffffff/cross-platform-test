#include "serverwidget.h"
#include "ui_serverwidget.h"

#include <QDateTime>

ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWidget)
{
    ui->setupUi(this);

    connect(&m_zTcpServer, &ZTcpServer::sgnDebugStr, [this](QString msg)
    {
        QString t = QTime::currentTime().toString("hh:mm:ss.zzz");
        ui->editMsg->appendPlainText(t + " " + msg);
    });
}

ServerWidget::~ServerWidget()
{
    delete ui;
}

void ServerWidget::init(QString tit, QString addr, int port)
{
    ui->lbTit->setText(tit);
    ui->editAddr->setText(addr);
    ui->editPort->setText(QString::number(port));
}

void ServerWidget::clear()
{
    ui->editMsg->clear();
}

void ServerWidget::on_btnRun_clicked()
{
    if(ui->btnRun->text() == "stop") {
        m_zTcpServer.Stop();

        ui->editAddr->setEnabled(true);
        ui->editPort->setEnabled(true);
        ui->btnRun->setText("run");
        return;
    }

    QString addr = ui->editAddr->text();
    int port = ui->editPort->text().toInt();
    bool ok = m_zTcpServer.Start(addr, port);
    if (!ok)
        return;

    ui->editAddr->setEnabled(false);
    ui->editPort->setEnabled(false);
    ui->btnRun->setText("stop");
}

void ServerWidget::on_btnClear_clicked()
{
    clear();
}
