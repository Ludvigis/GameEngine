#pragma once
#include "Application.h"


extern Engine::Application* Engine::CreateApplication();

int main()
{

	Engine::Application* app = Engine::CreateApplication();

	app->run();
	
	delete app;
}
