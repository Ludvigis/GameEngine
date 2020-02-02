#pragma once
#include "vulkan/vulkan.h"
#include <vector>

namespace Engine {

	class VulkanSwapchain {
	public:
		VulkanSwapchain(VkInstance instance);
		~VulkanSwapchain();
		void create(uint32_t width, uint32_t height);
		void cleanup();
		VkResult acquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex);
		VkResult queuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore);
		void createSurface();
		void connect(VkDevice device, VkPhysicalDevice physicalDevice);
		VkSurfaceKHR getSurface() { return surface; }
		VkFormat getColorFormat() { return colorFormat; }
		VkExtent2D getExtents() { return extents; }
		std::vector<VkImage> getImages() { return images; }
		std::vector<VkImageView> getImageViews() { return imageViews; }

	private:
		VkInstance instance;
		VkDevice device;
		VkPhysicalDevice physicalDevice;
		VkSurfaceKHR surface;

		VkSwapchainKHR swapchain = VK_NULL_HANDLE;

		uint32_t imageCount;
		std::vector<VkImage> images;
		std::vector<VkImageView> imageViews;
		VkFormat colorFormat;
		VkExtent2D extents;

		VkPresentModeKHR choosePresentMode();
		VkSurfaceFormatKHR chooseSwapSurfaceFormat();
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	};

}