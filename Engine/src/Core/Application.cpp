#include "Application.h"
#include "Log.h"
#include "GLFW/glfw3.h"

namespace Engine {

	Application* Application::instance = nullptr;

	Application::Application() {
		instance = this;

	}

	void Application::run()
	{
		window.reset(new Window(1280, 720, "Engine"));
		renderer.reset(new Renderer());
		
		while (!window->windowShouldClose()) {
			
			window->onUpdate();
			renderer->draw();

		}
	}



	Window& Application::getWindow() {
		return *window;
	}

}

