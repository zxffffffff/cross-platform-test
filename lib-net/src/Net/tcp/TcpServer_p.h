#pragma once
#include "TcpCommon.h"
#include <uv.h>

namespace Net {

class TcpServerPrivate : public TcpPrivate
{
public:
    std::atomic_bool running{ false };
    std::shared_ptr<uv_async_t> async_close = std::make_shared<uv_async_t>();
    std::shared_ptr<uv_async_t> async_write = std::make_shared<uv_async_t>();
    std::unordered_map<SocketPtr, std::vector<char>> write_map;
    std::mutex write_mutex;
private:
    std::shared_ptr<uv_loop_t> loop = std::make_shared<uv_loop_t>();
    std::shared_ptr<uv_tcp_t> server = std::make_shared<uv_tcp_t>();
    sockaddr_in addr{ 0 };
    int listen_backlog = 128;
    std::list<std::shared_ptr<uv_tcp_t>> clients;
    std::vector<char> read_buf;

public:
    TcpServerPrivate(const char* ip, int port, const char* logName)
        : TcpPrivate(ip, port, logName)
    {
    }

    static void onRun(void* arg)
    {
        auto priv = (TcpServerPrivate*)arg;
        uv_loop_init(priv->loop.get());

        do {
            uv_tcp_init(priv->loop.get(), priv->server.get());
            priv->server->data = priv;

            uv_ip4_addr(priv->ip.c_str(), priv->port, &priv->addr);

            uv_tcp_bind(priv->server.get(), (const struct sockaddr*)&priv->addr, 0);

            int r = uv_listen((uv_stream_t*)priv->server.get(), priv->listen_backlog, onConnection);
            if (r) {
                LOG(ERROR) << "[Listen error]" << priv->LogName() << " " << uv_err_name(r) << " " << uv_strerror(r);
                break;
            }
            priv->running = true;
            priv->clients.clear();
            priv->read_buf.reserve(4 * 1024);

            priv->async_close->data = priv;
            priv->async_write->data = priv;
            uv_async_init(priv->loop.get(), priv->async_close.get(), TcpServerPrivate::onAsyncClose);
            uv_async_init(priv->loop.get(), priv->async_write.get(), TcpServerPrivate::onAsyncWrite);

            LOG(INFO) << "[server run]" << priv->LogName();
            if (priv->handleRun)
                priv->handleRun();
            uv_run(priv->loop.get(), UV_RUN_DEFAULT);
            LOG(INFO) << "[server stop]" << priv->LogName();
        } while (0);

        uv_loop_close(priv->loop.get());
    }

    static void onAsyncClose(uv_async_t* handle)
    {
        auto priv = (TcpServerPrivate*)handle->data;
        LOG(INFO) << "[onAsyncClose]" << priv->LogName();

        uv_close((uv_handle_t*)priv->server.get(), TcpServerPrivate::onClose);
    }

    static void onClose(uv_handle_t* server)
    {
        auto priv = (TcpServerPrivate*)server->data;
        LOG(INFO) << "[onClose]" << priv->LogName();

        priv->running = false;
        if (priv->handleClose)
            priv->handleClose();
    }

    static void onConnection(uv_stream_t* server, int status)
    {
        auto priv = (TcpServerPrivate*)server->data;

        if (status < 0) {
            LOG(ERROR) << "[onConnection error]" << priv->LogName() << " " << uv_err_name(status) << " " << uv_strerror(status);
            return;
        }

        LOG(INFO) << "[onConnection]" << priv->LogName();

        priv->clients.push_back(std::make_shared<uv_tcp_t>());
        auto client = priv->clients.back();
        uv_tcp_init(priv->loop.get(), client.get());
        client->data = priv;

        if (uv_accept(server, (uv_stream_t*)client.get()) == 0) {
            if (priv->handleNewConn)
                priv->handleNewConn(client.get());

            uv_read_start((uv_stream_t*)client.get(), onReadAllocCbk, onReadCbk);
        }
        else {
            uv_close((uv_handle_t*)client.get(), onCloseConnection);
        }
    }

    static void onCloseConnection(uv_handle_t* client)
    {
        auto priv = (TcpServerPrivate*)client->data;
        LOG(INFO) << "[onCloseConnection]" << priv->LogName();

        for (auto ite = priv->clients.begin(); ite != priv->clients.end(); ++ite) {
            if (ite->get() == (uv_tcp_t*)client) {
                if (priv->handleCloseConn)
                    priv->handleCloseConn(client);

                priv->clients.erase(ite);
                break;
            }
        }
    }

    static void onReadAllocCbk(uv_handle_t* client, size_t suggested_size, uv_buf_t* buf)
    {
        auto priv = (TcpServerPrivate*)client->data;

        priv->read_buf.resize(suggested_size);
        *buf = uv_buf_init(priv->read_buf.data(), suggested_size);
    }

    static void onReadCbk(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf)
    {
        auto priv = (TcpServerPrivate*)client->data;

        if (nread > 0) {
            LOG(INFO) << "[onReadCbk]" << priv->LogName() << " " << "size=" << nread;

            if (priv->handleConnOnRead)
                priv->handleConnOnRead(client, buf->base, nread);
        }
        if (nread < 0 || nread == UV_EOF) {
            LOG(INFO) << "[onReadCbk close]" << priv->LogName() << " " << uv_err_name(nread) << " " << uv_strerror(nread);
            uv_close((uv_handle_t*)client, onCloseConnection);
        }
    }

    static void onAsyncWrite(uv_async_t* handle)
    {
        auto priv = (TcpServerPrivate*)handle->data;

        std::unordered_map<SocketPtr, std::vector<char>> write_map;
        {
            std::lock_guard<std::mutex> guard(priv->write_mutex);
            std::swap(write_map, priv->write_map);
        }

        for (auto ite = write_map.begin(); ite != write_map.end(); ++ite) {
            auto& socket = ite->first;
            auto& write_buf = ite->second;

            bool flag = false;
            for (auto ite = priv->clients.begin(); ite != priv->clients.end(); ++ite) {
                if (ite->get() == socket) {
                    flag = true;
                    break;
                }
            }
            if (!flag)
                continue;

            LOG(INFO) << "[onAsyncWrite]" << priv->LogName() << " " << "size=" << write_buf.size();
            auto req = new uv_write_t;
            auto bufs = uv_buf_init(write_buf.data(), write_buf.size());
            int r = ::uv_write(req, (uv_stream_t*)socket, &bufs, 1, [](uv_write_t* req, int status) { delete req; });
            if (r) {
                LOG(INFO) << "[onAsyncWrite error]" << priv->LogName() << " " << uv_err_name(r) << " " << uv_strerror(r);
                delete req;
            }
        }
    }
};

} // namespace Net
