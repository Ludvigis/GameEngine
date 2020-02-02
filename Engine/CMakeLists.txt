cmake_minimum_required(VERSION 3.13)


set(
	source_list_core
	src/Core/Application.cpp
    src/Core/Application.h
    src/Core/EntryPoint.h
    src/Core/Window.cpp
    src/Core/Window.h
	src/Core/Log.h
	src/Core/Log.cpp
)

set(
	source_list_renderer
	src/Renderer/RenderApi.h
	src/Renderer/RenderApi.cpp
	src/Renderer/Renderer.h
	src/Renderer/Renderer.cpp
)

set(
	source_list_platform
	src/Platform/Vulkan/LoadShader.h
	src/Platform/Vulkan/VulkanRenderApi.h
	src/Platform/Vulkan/VulkanRenderApi.cpp
	src/Platform/Vulkan/VulkanSwapchain.h
	src/Platform/Vulkan/VulkanSwapchain.cpp
	src/Platform/Vulkan/VulkanDevice.h
	src/Platform/Vulkan/VulkanDevice.cpp
	src/Platform/Vulkan/VulkanTexture.h
	src/Platform/Vulkan/VulkanTexture.cpp
	src/Platform/Vulkan/VulkanModel.h
	src/Platform/Vulkan/VulkanModel.cpp
	src/Platform/Vulkan/VulkanUtilities.h
	src/Platform/Vulkan/VulkanUtilities.cpp
)

set(
	source_list_shaders
	assets/shaders/shader.frag
	assets/shaders/shader.vert
	assets/shaders/vert.spv
	assets/shaders/frag.spv
)

set(
    source_list
	include/Engine.h
	${source_list_core}
	${source_list_renderer}
	${source_list_platform}
	${source_list_shaders}
)

add_library(engine STATIC ${source_list})

add_compile_definitions(ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")

target_link_libraries(engine glfw)
target_link_libraries(engine spdlog)
target_link_libraries(engine glm)


find_package(Vulkan REQUIRED)
target_include_directories(engine PRIVATE Vulkan::Vulkan)
target_link_libraries(engine Vulkan::Vulkan)


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${source_list})

target_include_directories(engine
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		${CMAKE_CURRENT_SOURCE_DIR}/src	#TODO private?
		${CMAKE_SOURCE_DIR}/thirdparty/stb
		${CMAKE_SOURCE_DIR}/thirdparty/tinyobjloader
)
