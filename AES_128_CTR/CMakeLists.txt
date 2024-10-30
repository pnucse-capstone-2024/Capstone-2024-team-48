cmake_minimum_required(VERSION 3.25)
project(AES_128_CTR C)

set(CMAKE_C_STANDARD 11)

add_executable(AES_128_CTR
        AES.c)
