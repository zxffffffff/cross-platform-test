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
    CORE_LIB_API void core_init(int argc, char *argv[]);

    // [1] 网络

    // Tcp Client
    typedef void *TcpClientPtr;

    typedef void *SocketPtr;
    typedef void (*HandleRun)();
    typedef void (*HandleClose)();
    typedef void (*HandleNewConn)(SocketPtr socket_ptr);
    typedef void (*HandleCloseConn)(SocketPtr socket_ptr);
    typedef void (*HandleConnOnRead)(SocketPtr socket_ptr, const char *buf, size_t len);

    CORE_LIB_API TcpClientPtr new_tcp_client(
        const char *ip,
        int port,
        HandleRun fRun = nullptr,
        HandleClose fClose = nullptr,
        HandleNewConn fNewConn = nullptr,
        HandleCloseConn fCloseConn = nullptr,
        HandleConnOnRead fConnOnRead = nullptr);

    CORE_LIB_API void del_tcp_client(TcpClientPtr tcpClient);

    CORE_LIB_API void tcp_client_write(
        TcpClientPtr tcpClient,
        const char *buf,
        int len);

    // Tcp Server pingpong
    CORE_LIB_API void tcp_server_pingpong(
        const char *ip,
        int port,
        const char *logName = nullptr);

    // Http Server "/pingpong" "data"
    CORE_LIB_API void http_server_pingpong(
        const char *ip,
        int port,
        const char *logName = nullptr);

    // [2] 结束退出
    CORE_LIB_API void core_shutdown();

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // CORE_LIB_H