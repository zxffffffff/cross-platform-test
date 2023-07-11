#include "ztcpclient.h"

QString formatBytes(int len)
{
    if (len > 1024)
        return QString::number(len / 1024.0f, 'f', 1) + " KB";
    if (len > 1024 * 1024)
        return QString::number(len / 1024 / 1024.0f, 'f', 1) + " MB";
    return QString::number(len) + " B";
}

ZTcpClient::ZTcpClient(QObject *parent)
    : QObject{parent}
{

}

ZTcpClient::~ZTcpClient()
{

}

bool ZTcpClient::Start(QString addr, int port)
{
    m_addr = addr;
    m_port = port;

    m_tcpSocket = std::make_shared<QTcpSocket>(this);
    connect(m_tcpSocket.get(), &QTcpSocket::stateChanged, this, &ZTcpClient::slotStateChanged);
    connect(m_tcpSocket.get(), &QTcpSocket::readyRead, this, &ZTcpClient::slotReadyRead);

    m_tcpSocket->connectToHost(QHostAddress(m_addr), m_port);
    bool ok = m_tcpSocket->waitForConnected(1 * 1000);

    emit sgnDebugStr("connect...");
    return ok;
}

bool ZTcpClient::Stop()
{
    if (!m_tcpSocket)
        return false;

    m_addr.clear();
    m_port = 0;

    m_tcpSocket->disconnectFromHost();

    emit sgnDebugStr("disconnect...");
    return true;
}

bool ZTcpClient::Send(QString msg)
{
    if (!m_tcpSocket)
        return false;
    if (!m_tcpSocket->isOpen())
        return false;

    QByteArray buf = msg.toLocal8Bit();
    auto len =  m_tcpSocket->write(buf);
    emit sgnDebugStr("send: " + formatBytes(buf.length()));

    return len;
}

void ZTcpClient::slotStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::SocketState::ClosingState) {
        emit sgnDebugStr("closeConnection --------");
    } else {
        emit sgnDebugStr("stateChanged:" + QVariant::fromValue(socketState).toString());
    }
}

void ZTcpClient::slotReadyRead()
{
    QByteArray buf = m_tcpSocket->readAll();
    emit sgnDebugStr("recv: " + formatBytes(buf.length()));
    emit sgnOnRead(buf);
}
