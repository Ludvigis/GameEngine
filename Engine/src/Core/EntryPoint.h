#pragma once
#include "Application.h"
#include "Log.h"

extern Engine::Application* Engine::CreateApplication();

int main()
{
	Engine::Log::init();
	Engine::Application* app = Engine::CreateApplication();

	app->run();
	
	delete app;


}
