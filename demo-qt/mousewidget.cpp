#include "mousewidget.h"

MouseWidget::MouseWidget(QWidget *parent)
    : QWidget{parent}
{
    setMouseTracking(true);
}

void MouseWidget::paintEvent(QPaintEvent *event)
{
    auto rect = this->rect();
    QPainter painter(this);
    painter.setPen(Qt::red);
    painter.drawLine(QPoint(pos.x(), rect.top()), QPoint(pos.x(), rect.bottom()));
    painter.drawLine(QPoint(rect.left(), pos.y()), QPoint(rect.right(), pos.y()));

    // 通过事件循环模拟获取耗时
    QTimer::singleShot(1, [=]{
        QPoint cursorPos = QCursor::pos();
        QPoint pos2 = mapFromGlobal(cursorPos);
        int dx = pos2.x() - pos.x();
        int dy = pos2.y() - pos.y();
        int distance = qSqrt(dx * dx + dy * dy); // 欧几里得距离，直线距离
        qDebug() << distance << " px";
    });
}

void MouseWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint cursorPos = QCursor::pos();
    pos = mapFromGlobal(cursorPos);
    update();
}
