#pragma once
#include "HttpCommon.h"
#include "httplib.h"

namespace Net
{

class HttpServerPrivate
{
public:
    std::string ip;
    int port;
    std::string logName;
    
    std::atomic_bool running{ false };

private:
    httplib::Server server;

public:
    HttpServerPrivate(const char *ip, int port, const char *logName = nullptr)
        : ip(ip), port(port), logName(logName ? logName : ip + std::string(":") + std::to_string(port))
    {
    }

    void HandleGet(const std::string &pattern, HandleReq f)
    {
        LOG(INFO) << "[HandleGet]" << logName << pattern;
        server.Get(pattern, [=](const httplib::Request& req, httplib::Response& res) {
            LOG(INFO) << "[Get]" << logName << pattern;
            std::map<std::string, std::string> params(req.params.begin(), req.params.end());
            auto ret = f(params);
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_content(ret, "text/plain");
        });
    }

    void HandlePost(const std::string &pattern, HandleReq f)
    {
        LOG(INFO) << "[HandlePost]" << logName << pattern;
        server.Post(pattern, [=](const httplib::Request& req, httplib::Response& res) {
            LOG(INFO) << "[Post]" << logName << pattern;
            std::map<std::string, std::string> params(req.params.begin(), req.params.end());
            auto ret = f(params);
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_content(ret, "text/plain");
        });
    }

    void run()
    {
        LOG(INFO) << "[server run]" << logName;
        running = true;
        bool ok = server.listen(ip, port);
        running = false;
        LOG(INFO) << "[server stop]" << logName;
    }

    void close()
    {
        LOG(INFO) << "[server close]" << logName;
        server.stop();
    }
};

} // namespace Net
