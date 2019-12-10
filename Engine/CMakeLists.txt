cmake_minimum_required(VERSION 3.13)


set(
    source_list
	include/Engine.h
    src/Core/Application.cpp
    src/Core/Application.h
    src/Core/EntryPoint.h
    src/Core/Window.cpp
    src/Core/Window.h
)

add_library(engine STATIC ${source_list})

target_link_libraries(engine glfw)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${source_list})

target_include_directories(engine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${CMAKE_CURRENT_SOURCE_DIR}/src	#TODO private?
)
