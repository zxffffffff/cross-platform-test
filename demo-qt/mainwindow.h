#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mousewidget.h"
#include "tcpclient.h"

#include <QMainWindow>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onRowColChanged();

    void on_btnRun_clicked();
    void on_btnReq_clicked();
    void on_btnTimer_clicked();

    void on_net_read(QByteArray& buf);

    void on_btnPop_clicked();

private:
    Ui::MainWindow *ui;
    MouseWidget* m_mouseWidget = nullptr;
    bool m_isTcp = false; // or http
    TcpClient m_tcpClient;
    QNetworkAccessManager m_httpManager;
    QTimer m_timer;
};

#endif // MAINWINDOW_H
