cmake_minimum_required(VERSION 3.13)

set(
    source_list
    "src/main.cpp"
    "src/Test.cpp"
    "src/Test.h"
)

add_executable(sandbox ${source_list})
