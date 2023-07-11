#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    ~TcpClient();

signals:
    void sgnOnRead(const char* buf, int len);

public slots:
    void run(const char* ip, int port);
    void close();
    void write(const char* buf, int len);

private:
    void* m_tcpClient = nullptr;
};

#endif // TCPCLIENT_H
