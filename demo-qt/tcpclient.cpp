#include "tcpclient.h"
#include "Core/Core.h"
#include <set>

std::set<TcpClient*> g_clients;

void onHandleRun()
{

}
void onHandleClose()
{

}
void onHandleNewConn(SocketPtr socket_ptr)
{

}
void onHandleCloseConn(SocketPtr socket_ptr)
{

}
void onHandleConnOnRead(SocketPtr socket_ptr, const char* buf, size_t len)
{
    // todo
    for (auto p : g_clients) {
        emit p->sgnOnRead(buf, len);
    }
}

TcpClient::TcpClient(QObject *parent)
    : QObject{parent}
{

}

TcpClient::~TcpClient()
{
    close();
}

void TcpClient::run(const char* ip, int port)
{
    if (m_tcpClient)
        return;

    m_tcpClient = new_tcp_client(
                ip, port,
                onHandleRun,
                onHandleClose,
                onHandleNewConn,
                onHandleCloseConn,
                onHandleConnOnRead);

    g_clients.insert(this);
}

void TcpClient::close()
{
    if (!m_tcpClient)
        return;

    del_tcp_client(m_tcpClient);
    m_tcpClient = nullptr;

    g_clients.erase(this);
}

void TcpClient::write(const char *buf, int len)
{
    if (!m_tcpClient)
        return;

    tcp_client_write(m_tcpClient, buf, len);
}
