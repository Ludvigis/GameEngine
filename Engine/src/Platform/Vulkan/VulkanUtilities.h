#pragma once
#include "vulkan/vulkan.h"
#include "Core/Log.h"
#include "VulkanDevice.h"

#define VK_CHECK_RESULT(func) {																\
	VkResult res = (func);																	\
	if (res != VK_SUCCESS) {																\
		LOG_ERROR("VkResult = {} at {} line: {}", res, __FILE__, __LINE__);					\
		assert(res == VK_SUCCESS);															\
	}																						\
}

namespace Engine {
	class VulkanUtilities {
	public:
		static void createImage(VulkanDevice vulkanDevice, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
			VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	};

}



