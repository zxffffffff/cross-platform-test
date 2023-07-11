#pragma once
#include "TcpCommon.h"
#include <uv.h>

namespace Net {

class TcpClientPrivate : public TcpPrivate
{
public:
    std::atomic_bool running{ false };
    std::shared_ptr<uv_async_t> async_close = std::make_shared<uv_async_t>();
    std::shared_ptr<uv_async_t> async_write = std::make_shared<uv_async_t>();
    std::vector<char> write_buf;
    std::mutex write_mutex;
private:
    std::shared_ptr<uv_loop_t> loop = std::make_shared<uv_loop_t>();
    std::shared_ptr<uv_tcp_t> socket = std::make_shared<uv_tcp_t>();
    sockaddr_in dest{ 0 };
    std::shared_ptr<uv_connect_t> connect = std::make_shared<uv_connect_t>();
    std::vector<char> read_buf;

public:
    TcpClientPrivate(const char* ip, int port, const char* logName)
        : TcpPrivate(ip, port, logName)
    {
    }

    static void onRun(void* arg)
    {
        auto priv = (TcpClientPrivate*)arg;
        uv_loop_init(priv->loop.get());

        do {
            uv_tcp_init(priv->loop.get(), priv->socket.get());
            priv->socket->data = priv;

            uv_ip4_addr(priv->ip.c_str(), priv->port, &priv->dest);

            int r = uv_tcp_connect(priv->connect.get(), priv->socket.get(), (const struct sockaddr*)&priv->dest, onConnection);
            if (r) {
                LOG(ERROR) << "[Connect error]" << priv->LogName() << " " << uv_err_name(r) << " " << uv_strerror(r);
                break;
            }
            priv->connect->data = priv;
            priv->running = true;
            priv->read_buf.reserve(4 * 1024);

            priv->async_close->data = priv;
            priv->async_write->data = priv;
            uv_async_init(priv->loop.get(), priv->async_close.get(), TcpClientPrivate::onAsyncClose);
            uv_async_init(priv->loop.get(), priv->async_write.get(), TcpClientPrivate::onAsyncWrite);

            LOG(INFO) << "[client run]" << priv->LogName();
            if (priv->handleRun)
                priv->handleRun();
            uv_run(priv->loop.get(), UV_RUN_DEFAULT);
            LOG(INFO) << "[client stop]" << priv->LogName();
        } while (0);

        uv_loop_close(priv->loop.get());
    }

    static void onAsyncClose(uv_async_t* handle)
    {
        auto priv = (TcpClientPrivate*)handle->data;
        LOG(INFO) << "[onAsyncClose]" << priv->LogName();

        uv_close((uv_handle_t*)priv->socket.get(), TcpClientPrivate::onClose);
    }

    static void onClose(uv_handle_t* socket)
    {
        auto priv = (TcpClientPrivate*)socket->data;
        LOG(INFO) << "[onClose]" << priv->LogName();

        priv->running = false;
        if (priv->handleClose)
            priv->handleClose();
    }

    static void onConnection(uv_connect_t* connect, int status)
    {
        auto priv = (TcpClientPrivate*)connect->data;

        if (status < 0) {
            LOG(ERROR) << "[onConnection error]" << priv->LogName() << " " << uv_err_name(status) << " " << uv_strerror(status);
            return;
        }

        LOG(INFO) << "[onConnection]" << priv->LogName();

        if (priv->handleNewConn)
            priv->handleNewConn(priv->socket.get());

        uv_read_start((uv_stream_t*)priv->socket.get(), onReadAllocCbk, onReadCbk);
    }

    static void onCloseConnection(uv_handle_t* connect)
    {
        auto priv = (TcpClientPrivate*)connect->data;
        LOG(INFO) << "[onCloseConnection]" << priv->LogName();

        if (priv->handleCloseConn)
            priv->handleCloseConn(priv->socket.get());
    }

    static void onReadAllocCbk(uv_handle_t* socket, size_t suggested_size, uv_buf_t* buf)
    {
        auto priv = (TcpClientPrivate*)socket->data;

        priv->read_buf.resize(suggested_size);
        *buf = uv_buf_init(priv->read_buf.data(), suggested_size);
    }

    static void onReadCbk(uv_stream_t* socket, ssize_t nread, const uv_buf_t* buf)
    {
        auto priv = (TcpClientPrivate*)socket->data;

        if (nread > 0) {
            LOG(INFO) << "[onReadCbk]" << priv->LogName() << " " << "size=" << nread;

            if (priv->handleConnOnRead)
                priv->handleConnOnRead(socket, buf->base, nread);
        }
        if (nread < 0 || nread == UV_EOF) {
            LOG(INFO) << "[onReadCbk close]" << priv->LogName() << " " << uv_err_name(nread) << " " << uv_strerror(nread);
            uv_close((uv_handle_t*)socket, onCloseConnection);
        }
    }

    static void onAsyncWrite(uv_async_t* handle)
    {
        auto priv = (TcpClientPrivate*)handle->data;

        std::vector<char> write_buf;
        {
            std::lock_guard<std::mutex> guard(priv->write_mutex);
            std::swap(write_buf, priv->write_buf);
        }

        if (write_buf.empty())
            return;

        LOG(INFO) << "[onAsyncWrite]" << priv->LogName() << " " << "size=" << write_buf.size();
        auto req = new uv_write_t;
        auto bufs = uv_buf_init(write_buf.data(), write_buf.size());
        int r = ::uv_write(req, (uv_stream_t*)priv->socket.get(), &bufs, 1, [](uv_write_t* req, int status) { delete req; });
        if (r) {
            LOG(ERROR) << "[onAsyncWrite error]" << priv->LogName() << " " << uv_err_name(r) << " " << uv_strerror(r);
            delete req;
        }
    }
};

} // namespace Net
