#pragma once
#include "HttpCommon.h"

namespace Net {

class HttpServerPrivate;
class HttpServer
{
public:
    HttpServer(const char* ip, int port, const char* logName = nullptr);
    ~HttpServer();

    void HandleGet(const std::string& pattern, HandleReq f);
    void HandlePost(const std::string& pattern, HandleReq f);
    
    void Run();
    void Close();
    bool IsRunning() const;
    
private:
    std::unique_ptr<HttpServerPrivate> priv;
};

} // namespace Net
