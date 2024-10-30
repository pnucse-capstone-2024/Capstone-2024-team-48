cmake_minimum_required(VERSION 3.25)
project(Algorithm_Lightweight_tool C)

set(CMAKE_C_STANDARD 11)

include_directories(Algorithm_Lightweight_tool/headerFile)

add_executable(Algorithm_Lightweight_tool
        Algorithm_Lightweight_tool/headerFile/AES_128_CTR.h
        Algorithm_Lightweight_tool/headerFile/Conversion.h
        Algorithm_Lightweight_tool/main.c)
