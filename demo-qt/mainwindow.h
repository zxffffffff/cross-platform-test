#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ztcpclient.h"
#include <QMainWindow>

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
    void on_btnPush_clicked();

    void on_tcp_read(QString buf);

private:
    Ui::MainWindow *ui;
    ZTcpClient m_zTcpClient;
};
#endif // MAINWINDOW_H
