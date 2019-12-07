#pragma once
#include "Window.h"
#include <memory>

namespace Engine {


	class Application
	{
		public:
			void run();

		private:
			std::unique_ptr<Window> window;
	};

	Application* CreateApplication();

}
