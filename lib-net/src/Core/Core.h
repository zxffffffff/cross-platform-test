#ifdef _WIN32
#ifndef CORE_LIB_EXPORTS
#define CORE_LIB_API __declspec(dllexport)
#else
#define CORE_LIB_API __declspec(dllimport)
#endif
#else
#define CORE_LIB_API __attribute__((visibility("default")))
#endif

#ifndef CORE_LIB_H
#define CORE_LIB_H

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

    // [0] 初始化
    CORE_LIB_API void core_init(int argc, char* argv[]);

    // [1] 网络
    
    // Tcp Client
    typedef void* TcpClientPtr;
    typedef void* SocketPtr;
    typedef void (*HandleRun)();
    typedef void (*HandleClose)();
    typedef void (*HandleNewConn)(SocketPtr id);
    typedef void (*HandleCloseConn)(SocketPtr id);
    typedef void (*HandleConnOnRead)(SocketPtr id, const char* buf, size_t len);

    CORE_LIB_API TcpClientPtr new_tcp_client(
        const char* ip, 
        int port, 
        const char* logName = nullptr
    );

    CORE_LIB_API void tcp_client_run(
        TcpClientPtr tcpClient,
        HandleRun fRun = nullptr,
        HandleClose fClose = nullptr,
        HandleNewConn fNewConn = nullptr,
        HandleCloseConn fCloseConn = nullptr,
        HandleConnOnRead fConnOnRead = nullptr
    );
    
    CORE_LIB_API void tcp_client_close(TcpClientPtr tcpClient);
    
    CORE_LIB_API void del_tcp_client(TcpClientPtr tcpClient);

    // Tcp Server PingPong
    CORE_LIB_API void pingpong_server_run(
        const char* ip, 
        int port, 
        const char* logName = nullptr
    );
    
    // [2] 结束退出
    CORE_LIB_API void core_shutdown();

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // CORE_LIB_H