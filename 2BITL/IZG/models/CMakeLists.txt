cmake_minimum_required(VERSION 3.3)
project(models)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    _info.txt
    bunny.tri
    doomguy.bmp
    doomguy.tri
    koule.tri
    marvin.bmp
    marvin.tri
    ogro.bmp
    ogro.tri
    plane.bmp
    plane.tri
    skull.tri
    valec.tri
    yoshi.bmp
    yoshi.tri)

add_executable(models ${SOURCE_FILES})