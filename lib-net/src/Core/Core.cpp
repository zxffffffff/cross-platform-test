#include "Core.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "tcp/TcpClient.h"
#include "tcp/TcpServer.h"

void core_init(int argc, char* argv[])
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
    const char* ip, 
    int port, 
    const char* logName
) {
    LOG(INFO) << "new_tcp_client";

    auto p = new Net::TcpClient(ip, port, logName);
    return p;
}

void tcp_client_run(
    TcpClientPtr tcpClient,
    HandleRun fRun,
    HandleClose fClose,
    HandleNewConn fNewConn,
    HandleCloseConn fCloseConn,
    HandleConnOnRead fConnOnRead
) {
    LOG(INFO) << "tcp_client_run";

    auto p = static_cast<Net::TcpClient*>(tcpClient);
    p->SetHandleRun(fRun);
    p->SetHandleClose(fClose);
    p->SetHandleNewConn(fNewConn);
    p->SetHandleCloseConn(fCloseConn);
    p->SetHandleConnOnRead(fConnOnRead);
    p->Run();
}

void tcp_client_close(TcpClientPtr tcpClient)
{
    LOG(INFO) << "tcp_client_close";

    auto p = static_cast<Net::TcpClient*>(tcpClient);
    p->Close();
}

void del_tcp_client(TcpClientPtr tcpClient)
{
    LOG(INFO) << "del_tcp_client";

    auto p = static_cast<Net::TcpClient*>(tcpClient);
    delete p;
}

void pingpong_server_run(
    const char* ip, 
    int port, 
    const char* logName
) {
    static auto p = std::make_unique<Net::TcpServer>(ip, port, logName);
    p->SetHandleConnOnRead([](SocketPtr id, const char* buf, size_t len){
        p->Write(id, buf, len);
        LOG(WARNING) << "ping-pong len: " << len; 
    });
    p->Run();
}
