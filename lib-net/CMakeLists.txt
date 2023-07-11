message("[cpp-deps-vcpkg] -------- start")
cmake_minimum_required (VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

if (WIN32)
    # ...
else()
    # APPLE 和 UNIX 需要设置-fPIC
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

# 使用 vcpkg 工具链，需要在 project 之前设置
set(CMAKE_TOOLCHAIN_FILE "./vcpkg/scripts/buildsystems/vcpkg.cmake")
message(STATUS "[cpp-deps-vcpkg] CMAKE_TOOLCHAIN_FILE : ${CMAKE_TOOLCHAIN_FILE}")

project ("cpp-deps-vcpkg")

message(STATUS "[cpp-deps-vcpkg] PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}")

# src
include_directories(${PROJECT_SOURCE_DIR}/src/Common)
include_directories(${PROJECT_SOURCE_DIR}/src/Net)
include_directories(${PROJECT_SOURCE_DIR}/src/Core)
include_directories(${PROJECT_SOURCE_DIR}/src/Main)
add_subdirectory(src/Common)
add_subdirectory(src/Net)
add_subdirectory(src/Core)
add_subdirectory(src/Main)

message("[cpp-deps-vcpkg] -------- done")
