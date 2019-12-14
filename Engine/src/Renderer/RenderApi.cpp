#include "RenderApi.h"
#include "Platform/Vulkan/VulkanRenderApi.h"

namespace Engine {
	
	std::unique_ptr<RenderApi> RenderApi::create()
	{
		return std::make_unique<VulkanRenderApi>();
	}

	RenderApi::~RenderApi() {

	}

}
