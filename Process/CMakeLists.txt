cmake_minimum_required(VERSION 3.10)
project(ProcessOn C)

set(SOURCES
main.c
process.c)

add_executable(ProcessOn ${SOURCES})
add_compile_definitions(N=2)

add_executable(Fork fk.c)