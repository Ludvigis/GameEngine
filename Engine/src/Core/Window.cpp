#include "Window.h"
#include "Log.h"

namespace Engine {


	Window::Window(int width, int height, const std::string& title)
	{
		int initSuccess = glfwInit();

		if (initSuccess != 1) {
			LOG_ERROR("Failed to initialize GLFW");
		}
			
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
