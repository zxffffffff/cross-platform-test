#include "ztcpserver.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QTimer>

QJsonArray genArray(int row, int col)
{
    static int64_t flag = 0;
    QJsonArray jsonArray;
    for (int i = 0; i < row; ++i) {
        QString item;
        for (int j = 0; j < col; ++j) {
            if (j > 0)
                item.append(' ');
            item.append(QString::number(flag + (i * j * 1000000)));
        }
        jsonArray.append(item);
    }
    ++flag;
    return jsonArray;
}

ZTcpServer::ZTcpServer(QObject *parent)
    : QObject{parent}
{
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ZTcpServer::slotTimeout);
    timer->start(16); // 60 FPS
}

ZTcpServer::~ZTcpServer()
{

}

bool ZTcpServer::Start(QString addr, int port)
{
    m_addr = addr;
    m_port = port;

    m_tcpServer = std::make_shared<QTcpServer>(this);
    m_tcpClients.clear();

    connect(m_tcpServer.get(), &QTcpServer::newConnection, this, &ZTcpServer::slotNewConn);
    bool ok = m_tcpServer->listen(QHostAddress(m_addr), m_port);

    emit sgnDebugStr("listen...");
    return ok;
}

bool ZTcpServer::Stop()
{
    if (!m_tcpServer)
        return false;

    m_addr.clear();
    m_port = 0;

    m_tcpServer = nullptr;
    m_tcpClients.clear();

    emit sgnDebugStr("stop...");
    return true;
}

bool ZTcpServer::Send(QTcpSocket* socket, QString msg)
{
    if (m_tcpClients.find(socket) == m_tcpClients.end())
        return false;

    QByteArray buf = msg.toLocal8Bit();
    auto len =  socket->write(buf);
    emit sgnDebugStr("send: " + msg);
    return len;
}

void ZTcpServer::emitDebugStr(QTcpSocket* socket, QString msg)
{
    QString socketMsg = socket->localAddress().toString();
    emit sgnDebugStr("[" + socketMsg + "]" + msg);
}

void ZTcpServer::slotTimeout()
{
    if (m_tcpPush.empty())
        return;

    QJsonObject jsonObject;
    jsonObject["data"] = genArray(m_pushRow, m_pushCol);
    QJsonDocument jsonDocument(jsonObject);
    QString jsonString = jsonDocument.toJson(QJsonDocument::Indented);

    for (auto ite = m_tcpPush.begin(); ite != m_tcpPush.end(); ++ite) {
        QTcpSocket* socket = *ite;
        Send(socket, jsonString);
    }
}

void ZTcpServer::slotNewConn()
{
    QTcpSocket* socket = m_tcpServer->nextPendingConnection();
    m_tcpClients.insert(socket);

    connect(socket, &QAbstractSocket::stateChanged, this, &ZTcpServer::slotStateChanged);
    connect(socket, &QIODevice::readyRead, this, &ZTcpServer::slotReadyRead);
    emitDebugStr(socket, "newConnection ++++++++");
}

void ZTcpServer::slotStateChanged(QAbstractSocket::SocketState socketState)
{
    auto ite = m_tcpClients.find((QTcpSocket*)sender());
    if (ite == m_tcpClients.end())
        return;
    QTcpSocket* socket = *ite;

    if (socketState == QAbstractSocket::SocketState::ClosingState) {
        emitDebugStr(socket, "closeConnection --------");
        m_tcpClients.erase(ite);
        m_tcpPush.erase(socket);
    } else {
        emitDebugStr(socket, "stateChanged:" + QVariant::fromValue(socketState).toString());
    }
}

void ZTcpServer::slotReadyRead()
{
    auto ite = m_tcpClients.find((QTcpSocket*)sender());
    if (ite == m_tcpClients.end())
        return;
    QTcpSocket* socket = *ite;

    QByteArray buf = socket->readAll();
    emitDebugStr(socket, "recv: " + buf);

    QJsonDocument jsonDocument = QJsonDocument::fromJson(buf);
    QJsonObject jsonObject = jsonDocument.object();
    const QString api = jsonObject["api"].toString();
    const int row = jsonObject["row"].toInt();
    const int col = jsonObject["col"].toInt();

    if (api == "req") {
        QJsonObject jsonObject;
        jsonObject["data"] = genArray(row, col);
        QJsonDocument jsonDocument(jsonObject);
        QString jsonString = jsonDocument.toJson(QJsonDocument::Indented);
        Send(socket, jsonString);
    } else if (api == "push") {
        m_pushRow = row;
        m_pushCol = col;

        auto ite = m_tcpPush.find(socket);
        if (ite != m_tcpPush.end())
            m_tcpPush.erase(ite);
        else
            m_tcpPush.insert(socket);
    }
}
