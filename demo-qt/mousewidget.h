#ifndef MOUSEWIDGET_H
#define MOUSEWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QCursor>
#include <QTimer>

class MouseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MouseWidget(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    QPoint pos;
};

#endif // MOUSEWIDGET_H
