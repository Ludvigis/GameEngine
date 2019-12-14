#include "Renderer.h"
#include "Core/Log.h"

namespace Engine {

	Renderer::Renderer()
	{
		renderApi = RenderApi::create();
		renderApi->init();

	}

	void Renderer::draw() {
		renderApi->draw();
	}

}
