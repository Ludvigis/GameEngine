#pragma once
#include "vulkan/vulkan.h"
#include <vector>
#include <optional>
#include <string>

namespace Engine {

	class VulkanDevice {
		

	public:
		struct QueueFamilyIndices {
			std::optional<uint32_t> graphicsFamily;

			bool isComplete() {
				return graphicsFamily.has_value();
			}
		} queueFamilyIndices;

		VulkanDevice(VkPhysicalDevice device);
		VkPhysicalDeviceProperties getPhysicalDeviceProperties();
		VkPhysicalDeviceFeatures getPhysicalDeviceFeatures();
		VkPhysicalDeviceMemoryProperties getPhysicalDeviceMemoryProperties();
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		VkResult createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		VkDevice getDevice();
		QueueFamilyIndices getQueueFamilyIndices();
		VkPhysicalDevice getPhysicalDevice() { return physicalDevice; }
		void cleanup();

		VkPhysicalDevice physicalDevice;
		VkDevice device;
	private:
		VkPhysicalDeviceProperties physicalDeviceProperties;
		VkPhysicalDeviceFeatures physicalDeviceFeatures;
		VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
		std::vector<VkQueueFamilyProperties> queueFamilyProperties;
		std::vector<std::string> supportedExtensions;
		

		void createLogicalDevice(std::vector<const char*> deviceExtensions);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		

	};

}