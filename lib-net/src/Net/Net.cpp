#include "Net.h"
#include "uv.h"
#include "curl/curl.h"

using namespace Net;

void Net::Test()
{
    std::cout << "Hello Net." << std::endl;

    std::cout << "libuv version: " << uv_version_string() << std::endl;

    std::cout << "curl version: " << curl_version() << std::endl;
}
