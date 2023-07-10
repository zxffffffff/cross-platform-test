#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include "ztcpserver.h"

namespace Ui {
class ServerWidget;
}

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWidget(QWidget *parent = nullptr);
    ~ServerWidget();

    void init(QString tit, QString addr, int port);
    void clear();

public slots:
    void on_btnRun_clicked();
    void on_btnClear_clicked();

private:
    Ui::ServerWidget *ui;
    ZTcpServer m_zTcpServer;
};

#endif // SERVERWIDGET_H
