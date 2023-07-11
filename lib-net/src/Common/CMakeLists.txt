message("[Common] -------- start")
project(Common)

message(STATUS "[Common] PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}")

# 引入zlib库
find_package(ZLIB REQUIRED)
message(STATUS "[Common] ZLIB_VERSION_STRING : ${ZLIB_VERSION_STRING}")
message(STATUS "[Common] ZLIB_INCLUDE_DIRS : ${ZLIB_INCLUDE_DIRS}")
message(STATUS "[Common] ZLIB_LIBRARIES : ${ZLIB_LIBRARIES}")

# 引入 gflags
find_package(gflags CONFIG REQUIRED)

# 引入 glog
find_package(glog CONFIG REQUIRED)

# 静态库
add_library(${PROJECT_NAME} STATIC
    Common.cpp
    Common.h
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC gflags::gflags
    PUBLIC glog::glog
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

message("[Common] -------- done")
