#ifndef ZTCPSERVER_H
#define ZTCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <memory>
#include <set>

class ZTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit ZTcpServer(QObject *parent = nullptr);
    virtual ~ZTcpServer();

signals:
    void sgnDebugStr(QString msg);

public:
    bool Start(QString addr, int port);
    bool Stop();
    bool Send(QTcpSocket* socket, QString msg);

protected:
    void emitDebugStr(QTcpSocket* socket, QString msg);

protected slots:
    void slotTimeout();

    void slotNewConn();

    void slotStateChanged(QAbstractSocket::SocketState socketState);
    void slotReadyRead();

private:
    QString m_addr;
    int m_port = 0;

    std::shared_ptr<QTcpServer> m_tcpServer;
    std::set<QTcpSocket*> m_tcpClients;

    int m_pushRow = 0;
    int m_pushCol = 0;
    std::set<QTcpSocket*> m_tcpPush;
};

#endif // ZTCPSERVER_H
