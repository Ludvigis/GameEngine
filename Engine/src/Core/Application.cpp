#include "Application.h"
#include "Log.h"

namespace Engine {

	void Application::run()
	{
		window.reset(new Window(1280, 720, "Engine"));

		while (true) {
			
			window->onUpdate();

		}
	}

}

