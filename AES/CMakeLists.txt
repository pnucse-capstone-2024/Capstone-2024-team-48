cmake_minimum_required(VERSION 3.25)
project(AES C)

set(CMAKE_C_STANDARD 11)

add_executable(AES
    AES.c FileIO.h Substitution.h AES_encryption.h AES_decryption.h Calculate.h DataTypes.h)
