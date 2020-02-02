#pragma once
#include "vulkan/vulkan.h"
#include <string>
#include "VulkanDevice.h"
#include <memory>

namespace Engine {

	//TODO Texture2D & Texture3D(cubemap)
	class VulkanTexture {
	public:

		void loadFromFile(std::string filePath, VkFormat format, std::shared_ptr<VulkanDevice> vulkanDevice, VkQueue copyQueue, VkCommandPool commandPool);
		void cleanup();
		VkDescriptorImageInfo descriptorInfo;

	private:
		std::shared_ptr<VulkanDevice> vulkanDevice;
		VkDevice device;
		VkPhysicalDevice physicalDevice;
		VkCommandPool commandPool;
		VkQueue graphicsQueue;

		VkImage image;
		VkImageView imageView;
		VkDeviceMemory imageMemory;
		VkSampler sampler;

		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	};

}