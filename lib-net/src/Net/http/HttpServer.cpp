#include "HttpServer.h"
#include "HttpServer_p.h"

using namespace Net;

std::once_flag flag;

HttpServer::HttpServer(const char* ip, int port, const char* logName)
    : priv(std::make_unique<HttpServerPrivate>(ip, port, logName))
{
    
}

HttpServer::~HttpServer()
{
    Close();
}

void HttpServer::HandleGet(const std::string& pattern, HandleReq f)
{
    return priv->HandleGet(pattern, f);
}

void HttpServer::HandlePost(const std::string& pattern, HandleReq f)
{
    return priv->HandlePost(pattern, f);
}

void HttpServer::Run()
{
    std::thread tr([this]{
        priv->run();
    });
    tr.detach();
}

void HttpServer::Close()
{
    priv->close();
}

bool HttpServer::IsRunning() const
{
    return priv->running;
}
