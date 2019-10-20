cmake_minimum_required(VERSION 3.15)
project(parseargs C)

set(CMAKE_C_STANDARD 99)

add_executable(parseargs main.c splitargs.c splitargs.h)