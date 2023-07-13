#pragma once
#include "glog/logging.h"
#include <memory>
#include <list>
#include <unordered_map>
#include <iostream>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>

namespace Net {

using SocketPtr = void*;

using HandleRun = std::function<void()>;
using HandleClose = std::function<void()>;
using HandleNewConn = std::function<void(SocketPtr id)>;
using HandleCloseConn = std::function<void(SocketPtr id)>;
using HandleConnOnRead = std::function<void(SocketPtr id, const char* buf, size_t len)>;

class TcpPrivate
{
public:
    std::string ip;
    int port;
    std::string logName;

    HandleRun handleRun;
    HandleClose handleClose;
    HandleNewConn handleNewConn;
    HandleCloseConn handleCloseConn;
    HandleConnOnRead handleConnOnRead;

    TcpPrivate(const char* ip, int port, const char* _logName)
        : ip(ip), port(port), logName(_logName ? _logName : ip + std::string(":") + std::to_string(port))
    {
    }

    virtual ~TcpPrivate() = default;

    const std::string& LogName() const
    {
        return logName;
    }
};

} // namespace Net
