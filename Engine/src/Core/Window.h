#pragma once
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
	private:
		
		GLFWwindow* windowHandle;
	};

}
