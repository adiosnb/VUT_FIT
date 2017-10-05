cmake_minimum_required(VERSION 3.6)
project(ipk1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES_CLIENT client.cpp ClientArgParser.h ClientArgParser.cpp Connection.h Connection.cpp
        HttpHeader.cpp HttpHeader.h)
set(SOURCE_FILES_SERVER server.cpp ServerArgParser.cpp ServerArgParser.h Connection.h Connection.cpp
        ServerFunctions.cpp ServerFunctions.h HttpHeader.h HttpHeader.cpp)

set(TEST_SERVER test_server.cpp ServerFunctions.h ServerFunctions.cpp)

add_executable(test_server_func ${TEST_SERVER})
add_executable(ftrest ${SOURCE_FILES_CLIENT})
add_executable(ftrestd ${SOURCE_FILES_SERVER})