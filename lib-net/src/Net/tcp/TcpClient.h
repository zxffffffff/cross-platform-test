#pragma once
#include "TcpCommon.h"

namespace Net {

class TcpClientPrivate;
class TcpClient
{
public:
    TcpClient(const char* ip, int port, const char* logName = nullptr);
    ~TcpClient();

    void SetHandleRun(HandleRun f);
    void SetHandleClose(HandleClose f);
    void SetHandleNewConn(HandleNewConn f);
    void SetHandleCloseConn(HandleCloseConn f);
    void SetHandleConnOnRead(HandleConnOnRead f);

    void Run();
    void Close();
    bool IsRunning() const;

    void Write(const char* buf, int len);

private:
    std::unique_ptr<TcpClientPrivate> priv;
};

} // namespace Net
