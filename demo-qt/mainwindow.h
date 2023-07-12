#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tcpclient.h"

#include <QMainWindow>
#include <QTimer>
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QCursor>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MouseWidget : public QWidget
{
    Q_OBJECT

public:
    MouseWidget() {
        setMouseTracking(true);
    }

    void paintEvent(QPaintEvent *event) {
        auto rect = this->rect();
        QPainter painter(this);
        painter.setPen(Qt::red);
        painter.drawLine(QPoint(pos.x(), rect.top()), QPoint(pos.x(), rect.bottom()));
        painter.drawLine(QPoint(rect.left(), pos.y()), QPoint(rect.right(), pos.y()));

        // 通过全局鼠标模拟获取延迟
        QTimer::singleShot(1, [=]{
            QPoint cursorPos = QCursor::pos();
            QPoint pos2 = mapFromGlobal(cursorPos);
            int dx = pos2.x() - pos.x();
            int dy = pos2.y() - pos.y();
            int distance = qSqrt(dx * dx + dy * dy); // 欧几里得距离，直线距离
            qDebug() << distance << " px";
        });
    }

    void mouseMoveEvent(QMouseEvent *event) {
        QPoint cursorPos = QCursor::pos();
        pos = mapFromGlobal(cursorPos);
        update();
    }

    QPoint pos;
};

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

    void on_btnPop_clicked();

private:
    Ui::MainWindow *ui;
    MouseWidget* m_mouseWidget;
    TcpClient m_tcpClient;
    QTimer m_timer;
};

#endif // MAINWINDOW_H
