#include "Core.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "tcp/TcpClient.h"
#include "tcp/TcpServer.h"
#include "http/HttpServer.h"

void core_init(int argc, char *argv[])
{
    std::stringstream ss;
    ss << "core_init";
    ss << " argc=" << argc;
    for (int i = 0; i < argc; ++i)
        ss << " argv[" << std::to_string(i) << "]=" << argv[i];

    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);

    LOG(INFO) << ss.str();
}

void core_shutdown()
{
    LOG(INFO) << "core_shutdown";

    google::ShutdownGoogleLogging();
    gflags::ShutDownCommandLineFlags();
}

TcpClientPtr new_tcp_client(
    const char *ip,
    int port,
    HandleRun fRun,
    HandleClose fClose,
    HandleNewConn fNewConn,
    HandleCloseConn fCloseConn,
    HandleConnOnRead fConnOnRead)
{
    LOG(INFO) << "new_tcp_client";

    auto p = new Net::TcpClient(ip, port);
    p->SetHandleRun(fRun);
    p->SetHandleClose(fClose);
    p->SetHandleNewConn(fNewConn);
    p->SetHandleCloseConn(fCloseConn);
    p->SetHandleConnOnRead(fConnOnRead);
    p->Run();
    return p;
}

void del_tcp_client(TcpClientPtr tcpClient)
{
    LOG(INFO) << "del_tcp_client";

    auto p = static_cast<Net::TcpClient *>(tcpClient);
    p->Close();
    delete p;
}

void tcp_client_write(
    TcpClientPtr tcpClient,
    const char *buf,
    int len)
{
    LOG(INFO) << "tcp_client_write";

    auto p = static_cast<Net::TcpClient *>(tcpClient);
    p->Write(buf, len);
}

void tcp_server_pingpong(
    const char *ip,
    int port,
    const char *logName)
{
    static auto p = std::make_unique<Net::TcpServer>(ip, port, logName);
    p->SetHandleConnOnRead([](SocketPtr socket_ptr, const char *buf, size_t len) {
        p->Write(socket_ptr, buf, len);
    });
    p->Run();
}

void http_server_pingpong(
    const char *ip,
    int port,
    const char *logName)
{
    static auto p = std::make_unique<Net::HttpServer>(ip, port, logName);
    p->HandleGet("/pingpong", [](std::map<std::string, std::string> params){
        std::string ret = params["data"];
        return ret;
    });
    p->Run();
}
