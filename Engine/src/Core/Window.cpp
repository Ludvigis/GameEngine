#include "Window.h"
#include <iostream>

namespace Engine {


	Window::Window(int width, int height, const std::string& title)
	{
		int initSuccess = glfwInit();

		if(initSuccess != 1)
			std::cerr << "Failed to initialize GLFW" << std::endl;
	
		windowHandle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		glfwMakeContextCurrent(windowHandle);

	}

	Window::~Window() {
		glfwDestroyWindow(windowHandle);
		glfwTerminate();
	}

	void Window::onUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(windowHandle);
	}

}
