cmake_minimum_required(VERSION 3.13)

set(
    source_list
    "src/Sandbox.cpp"
)


add_executable(sandbox ${source_list})

target_link_libraries(sandbox engine)
