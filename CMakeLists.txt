cmake_minimum_required(VERSION 3.20)
project(irc)

FILE(GLOB SRC
        "src/main.cpp",
        "src/server/*.cpp",
        "src/user/*.cpp",
        "src/utils/*.cpp",
        "src/handler/*.cpp",
        "src/handler/commands/*.cpp",
        "src/message/*.cpp"
        )

set(CMAKE_CXX_STANDARD 14 "${CMAKE_C_FLAGS} -std=c98 -Wall")
add_executable(irc ${SRC} src/handler/error_msg.cpp)
