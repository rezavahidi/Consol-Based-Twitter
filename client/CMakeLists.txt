cmake_minimum_required(VERSION 3.16.5)
project(project C)

set(CMAKE_C_STANDARD 99)

add_executable(project main.c slre.c)
target_link_libraries(project ws2_32)