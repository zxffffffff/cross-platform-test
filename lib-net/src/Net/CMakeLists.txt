message("[Net] -------- start")
project(Net)

message(STATUS "[Net] PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}")

# 引入libuv库
find_package(libuv CONFIG REQUIRED)

# 引入curl库
find_package(CURL CONFIG REQUIRED)
message(STATUS "[Net] CURL_VERSION_STRING : ${CURL_VERSION_STRING}")
message(STATUS "[Net] CURL_INCLUDE_DIR : ${CURL_INCLUDE_DIR}")
message(STATUS "[Net] CURL_LIBRARIES : ${CURL_LIBRARIES}")

# 引入httplib库，也可使用header-only
find_package(httplib CONFIG REQUIRED)

# 静态库
add_library(${PROJECT_NAME} STATIC
    Net.cpp
    Net.h
    "tcp/TcpCommon.h"
    "tcp/TcpServer.cpp"
    "tcp/TcpServer.h"
    "tcp/TcpClient.cpp"
    "tcp/TcpClient.h"
    "http/HttpCommon.h"
    "http/HttpClient.cpp"
    "http/HttpClient.h"
    "http/HttpServer.cpp"
    "http/HttpServer.h"
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC Common
    PRIVATE $<IF:$<TARGET_EXISTS:libuv::uv_a>,libuv::uv_a,libuv::uv>
    PRIVATE CURL::libcurl
    PRIVATE httplib::httplib
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}
)

install(TARGETS ${PROJECT_NAME}
    PUBLIC_HEADER DESTINATION include
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION bin
    ARCHIVE DESTINATION lib
)

message("[Net] -------- done")
