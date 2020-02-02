#pragma once
#include "RenderApi.h"
#include "GLFW/glfw3.h"

namespace Engine {

	class Renderer {

	public:
		Renderer();
		void draw();

	private:
		std::unique_ptr<RenderApi> renderApi;

	};

}
