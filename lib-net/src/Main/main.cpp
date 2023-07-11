#include "Core.h"

#include <string>

int main(int argc, char* argv[])
{
#if 1
    std::string argv_0 = "test";
    std::string argv_1 = "--logtostderr=1";
    const char* argv_[2] = { argv_0.c_str(), argv_1.c_str() };
    core_init(2, (char**)argv_);

    pingpong_server_run("127.0.0.1", 8888);

    while (getchar() != 'q')
        continue;

    core_shutdown();
#endif
    return 0;
}
