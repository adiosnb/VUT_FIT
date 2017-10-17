cmake_minimum_required(VERSION 3.6)
project(proj2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-pthread -DIPK_IPV6")

set(SOURCE_FILES main.cpp ArpScannerParser.cpp ArpScannerParser.h XmlCoder.h XmlCoder.cpp
        Scanner.h Scanner.cpp NetStruct.h NetTools.cpp)
add_executable(ipk-scanner ${SOURCE_FILES})