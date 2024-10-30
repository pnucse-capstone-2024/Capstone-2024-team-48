cmake_minimum_required(VERSION 3.25)
project(AES_Algorithm_Set C)

set(CMAKE_C_STANDARD 11)

add_executable(AES_Algorithm_Set main.c AES128.h AES128_beforehand.h AES128_beforehand_light.h Common_Calculate.h AES128_RemoveShift.h AES128_Mixcolumns_Light.h)
