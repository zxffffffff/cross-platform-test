#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tcpclient.h"

#include <QMainWindow>
#include <QTimer>

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

    void on_tcp_read(const char* buf, int len);

private:
    Ui::MainWindow *ui;
    TcpClient m_tcpClient;
    QTimer m_timer;
};
#endif // MAINWINDOW_H
