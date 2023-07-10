#ifndef ZTCPCLIENT_H
#define ZTCPCLIENT_H

#include <QTcpSocket>
#include <memory>

class ZTcpClient : public QObject
{
    Q_OBJECT
public:
    explicit ZTcpClient(QObject *parent = nullptr);
    virtual ~ZTcpClient();

signals:
    void sgnDebugStr(QString msg);
    void sgnOnRead(QString buf);

public:
    bool Start(QString addr, int port);
    bool Stop();
    bool Send(QString msg);

protected slots:
    void slotStateChanged(QAbstractSocket::SocketState socketState);
    void slotReadyRead();

private:
    QString m_addr;
    int m_port = 0;

    std::shared_ptr<QTcpSocket> m_tcpSocket;
};

#endif // ZTCPCLIENT_H
