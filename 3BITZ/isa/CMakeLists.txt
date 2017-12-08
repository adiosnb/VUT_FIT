cmake_minimum_required(VERSION 3.8)
project(isa)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-pedantic -Wall -DSERVER_LOGGING")

set(SOURCE_FILES src/main.cpp src/ArgParser.cpp src/ArgParser.h src/Exception.cpp src/Exception.h
        src/Connection.cpp src/Connection.h src/debug.h src/MailMsg.cpp src/MailMsg.h src/MailDir.cpp src/MailDir.h
        src/Pop3Server.cpp src/Pop3Server.h src/Server.h src/Server.cpp src/Host.cpp src/Host.h
        src/MsgBuffer.h src/MsgBuffer.cpp src/md5.h src/md5.cpp src/HostConnection.h src/HostConnection.cpp
        src/MailDirLogger.h src/MailDirLogger.cpp)
add_executable(isa ${SOURCE_FILES})


