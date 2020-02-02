#pragma once
#include <memory>
#include "Core/Window.h"

namespace Engine {

	class RenderApi {
	public:
		virtual ~RenderApi() = 0;	

		virtual void init() = 0;
		virtual void draw() = 0;

		static std::unique_ptr<RenderApi> create();
	};

}
