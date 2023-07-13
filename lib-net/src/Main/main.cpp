#include "Core.h"

#include <iostream>
#include <string>

#define HTTP_SERVER_ON 1
#define TCP_SERVER_ON 1

int main(int argc, char* argv[])
{
    std::string argv_0 = "test";
    std::string argv_1 = "--logtostderr=1";
    const char* argv_[2] = { argv_0.c_str(), argv_1.c_str() };
    core_init(2, (char**)argv_);

#if HTTP_SERVER_ON
    http_server_pingpong("127.0.0.1", 8889);
#endif

#if TCP_SERVER_ON
    tcp_server_pingpong("127.0.0.1", 8888);
#endif

    std::cout << "press 'q' to quit" << std::endl;
    while (getchar() != 'q')
        continue;
    core_shutdown();
    return 0;
}
