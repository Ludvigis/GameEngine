#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include "Window.h"

namespace Engine {


	class Window
	{
	public:
		Window(int height, int width, const std::string& title);
		~Window();
		void onUpdate();
		bool windowShouldClose();

		GLFWwindow* getNativeWindow() { return windowHandle; };

	private:
		
		GLFWwindow* windowHandle;

	};

}
