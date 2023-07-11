#include "HttpClient.h"
#include "HttpClient_p.h"

using namespace Net;

std::once_flag flag;

HttpClient::HttpClient()
{
    HttpClientPrivate::global_init();
}

HttpClient::~HttpClient()
{
    HttpClientPrivate::global_cleanup();
}

std::string HttpClient::Get(
    const std::string& url,
    const std::map<std::string, std::string>& params,
    int timeout /*= 10*/
)
{
    std::string complete_url = url + "?" + ParseParam(params);
    LOG(INFO) << "[Http GET] url=" << complete_url;
    auto r = HttpClientPrivate::Get(complete_url.c_str(), timeout);
    LOG(INFO) << "[Http GET] url=" << url << " r=" << r;
    return r;
}

std::string HttpClient::Post(
    const std::string& url,
    const std::map<std::string, std::string>& params,
    const std::string& body,
    int timeout /*= 10*/
)
{
    std::string complete_url = url + "?" + ParseParam(params);
    LOG(INFO) << "[Http POST] url=" << complete_url << " body=" << body;
    auto r = HttpClientPrivate::Post(complete_url.c_str(), body.c_str(), timeout);
    LOG(INFO) << "[Http POST] url=" << url << " r=" << r;
    return r;
}

std::string HttpClient::ParseParam(const std::map<std::string, std::string>& params)
{
    if (params.empty())
        return "";
    std::stringstream ss;
    for (auto ite = params.begin(); ite != params.end(); ++ite) {
        if (ite != params.begin())
            ss << "&";
        ss << ite->first << "=" << ite->second;
    }
    return ss.str();
}
