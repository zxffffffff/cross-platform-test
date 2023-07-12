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
    resize(1920, 1080);

    m_mouseWidget = new MouseWidget();
    ui->verticalLayout_tab2->insertWidget(0, m_mouseWidget);

    connect(ui->spinBox_row, &QSpinBox::textChanged, this, &MainWindow::onRowColChanged);
    connect(ui->spinBox_col, &QSpinBox::textChanged, this, &MainWindow::onRowColChanged);
    onRowColChanged();

    connect(&m_tcpClient, &TcpClient::sgnOnRead, this, &MainWindow::on_tcp_read);

    connect(&m_timer, &QTimer::timeout, this, [this]{ on_btnReq_clicked(); });

    ui->btnReq->setEnabled(false);
    ui->btnTimer->setEnabled(false);
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
        m_tcpClient.close();

        ui->editAddr->setEnabled(true);
        ui->editPort->setEnabled(true);
        ui->btnRun->setText("run");
        ui->btnReq->setEnabled(false);
        ui->btnTimer->setEnabled(false);
        return;
    }

    QString addr = ui->editAddr->text();
    int port = ui->editPort->text().toInt();
    m_tcpClient.run(addr.toLocal8Bit().data(), port);

    ui->editAddr->setEnabled(false);
    ui->editPort->setEnabled(false);
    ui->btnRun->setText("stop");
    ui->btnReq->setEnabled(true);
    ui->btnTimer->setEnabled(true);
}

QJsonArray genArray(int row, int col)
{
    static int64_t flag = 0;
    QJsonArray jsonArray;
    for (int i = 0; i < row; ++i) {
        QString item;
        for (int j = 0; j < col; ++j) {
            if (j > 0)
                item.append(' ');
            item.append(QString::number(flag + (i * j * 1000000)));
        }
        jsonArray.append(item);
    }
    ++flag;
    return jsonArray;
}

void MainWindow::on_btnReq_clicked()
{
    const int row = ui->spinBox_row->value();
    const int col = ui->spinBox_col->value();

    QDateTime start = QDateTime::currentDateTime();
    qint64 t_start = start.toMSecsSinceEpoch();

    QJsonObject jsonObject;

    jsonObject["time"] = t_start;
    jsonObject["data"] = genArray(row, col);

    QJsonDocument jsonDocument(jsonObject);
    QByteArray jsonString = jsonDocument.toJson(QJsonDocument::Indented);
    m_tcpClient.write(jsonString.data(), jsonString.length());
}

void MainWindow::on_btnTimer_clicked()
{
    m_timer.isActive() ? m_timer.stop()
                       : m_timer.start(10);
}

void MainWindow::on_tcp_read(const char* buf, int len)
{
    QByteArray jsonString(buf, len);
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString);
    QJsonObject jsonObject = jsonDocument.object();

    qint64 t_start = jsonObject["time"].toInteger();

    QJsonArray jsonArray = jsonObject["data"].toArray();
    const int row = jsonArray.size();
    int col = 0;
    for (int i = 0; i < row; ++i) {
        QString item = jsonArray.at(i).toString();
        auto list = item.split(' ');
        col = list.size();
        for (int j = 0; j < col; ++j) {
            auto p = ui->tableWidget->item(i, j);
            if (p) {
                p->setText(list[j]);
            }
        }
    }

    // 通过事件循环模拟获取耗时，可以理解为paintEvent
    QTimer::singleShot(1, [=]{
        QDateTime stop = QDateTime::currentDateTime();
        qint64 t_stop = stop.toMSecsSinceEpoch();

        qDebug() << tr("%1 ms").arg(t_stop - t_start);
    });
}

void MainWindow::on_btnPop_clicked()
{
    MainWindow *window = new MainWindow();
    window->show();
}
