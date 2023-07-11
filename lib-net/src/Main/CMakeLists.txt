message("[Main] -------- start")
project ("Main")

message(STATUS "[Main] PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}")

# 将源代码添加到此项目的可执行文件。
add_executable (${PROJECT_NAME}
    "main.cpp"
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC Core
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

message("[Main] -------- done")