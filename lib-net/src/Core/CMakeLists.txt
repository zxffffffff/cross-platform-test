message("[Core] -------- start")
project (Core)

message(STATUS "[Core] PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}")

# dll导出宏
add_compile_definitions(CORE_LIB_EXPORTS)

# 将源代码添加到此项目的可执行文件。
add_library (${PROJECT_NAME} SHARED
    Core.h
    Core.cpp
)

# 动态库
target_link_libraries(${PROJECT_NAME}
    PUBLIC Net
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

message("[Core] -------- done")
