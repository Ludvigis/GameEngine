#pragma once
#include "Window.h"
#include <memory>
#include "Renderer/Renderer.h"

namespace Engine {


	class Application
	{
		public:
			Application();
			void run();

			static Application& get() { return *instance; }

			Window& getWindow();

		private:
			std::unique_ptr<Window> window;
			std::unique_ptr<Renderer> renderer;

			static Application* instance;

	};

	Application* CreateApplication();

}
