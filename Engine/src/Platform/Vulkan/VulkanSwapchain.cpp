#include "VulkanSwapchain.h"
#include "Core/Application.h"
#include <stdexcept>
#include "Core/Log.h"
#include "VulkanUtilities.h"

namespace Engine {


	VulkanSwapchain::VulkanSwapchain(VkInstance instance) : instance(instance){

	}

	VulkanSwapchain:: ~VulkanSwapchain() {

	}

	void VulkanSwapchain:: createSurface() {
		GLFWwindow* window = Application::get().getWindow().getNativeWindow();
		VK_CHECK_RESULT(glfwCreateWindowSurface(instance, window, nullptr, &surface));
	}

	void VulkanSwapchain:: create(uint32_t width, uint32_t height) {
		VkSwapchainKHR oldSwapchain = swapchain;

		VkSurfaceCapabilitiesKHR capabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

		VkPresentModeKHR presentMode = choosePresentMode();
		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat();
		colorFormat = surfaceFormat.format;
		extents = chooseSwapExtent(capabilities);

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;
		createInfo.minImageCount = capabilities.minImageCount + 1;
		createInfo.imageFormat = colorFormat;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extents;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.preTransform = capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;	//TODO not supported by all devices
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = oldSwapchain;

		VK_CHECK_RESULT(vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain));

		vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
		images.resize(imageCount);

		VK_CHECK_RESULT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.data()));

		imageViews.resize(images.size());

		for (size_t i = 0; i < imageViews.size(); i++) {
			VkImageView imageView;
			VkImageViewCreateInfo viewInfo = {};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = images[i];
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format = surfaceFormat.format;
			viewInfo.components = {
				VK_COMPONENT_SWIZZLE_R,
				VK_COMPONENT_SWIZZLE_G,
				VK_COMPONENT_SWIZZLE_B,
				VK_COMPONENT_SWIZZLE_A
			};
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			viewInfo.subresourceRange.baseMipLevel = 0;
			viewInfo.subresourceRange.levelCount = 1;
			viewInfo.subresourceRange.baseArrayLayer = 0;
			viewInfo.subresourceRange.layerCount = 1;
			
			vkCreateImageView(device, &viewInfo, nullptr, &imageView);
			imageViews[i] = imageView;
		}

	}

	VkPresentModeKHR VulkanSwapchain:: choosePresentMode() {
		VkPresentModeKHR chosenPresentMode;

		uint32_t presentModesCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModesCount, nullptr);

		std::vector<VkPresentModeKHR> presentModes;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModesCount, presentModes.data());

		for (VkPresentModeKHR presentMode : presentModes) {
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return presentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkSurfaceFormatKHR VulkanSwapchain::chooseSwapSurfaceFormat() {
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
		std::vector<VkSurfaceFormatKHR> formats;

		if (formatCount != 0) {
			formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());
		}

		for (const auto& availableFormat : formats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		LOG_WARN("Prefered SurfaceFormat not available. Picking first available format instead.");
		return formats[0];
	}

	VkExtent2D VulkanSwapchain:: chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {

		if (capabilities.currentExtent.width != UINT32_MAX) {
			return capabilities.currentExtent;
		}
		else {
			GLFWwindow* window = Application::get().getWindow().getNativeWindow();
			int width, height;

			glfwGetFramebufferSize(window, &width, &height);
			VkExtent2D actualExtent = { width, height };

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	VkResult VulkanSwapchain:: acquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex) {
		return vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, presentCompleteSemaphore, VK_NULL_HANDLE, imageIndex);
	}

	VkResult VulkanSwapchain::queuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore) {
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		
		VkSwapchainKHR swapChains[] = { swapchain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pWaitSemaphores = &waitSemaphore;
		presentInfo.waitSemaphoreCount = 1;

		VkResult result;
		return vkQueuePresentKHR(queue, &presentInfo);

	}

	void VulkanSwapchain:: connect(VkDevice device, VkPhysicalDevice physicalDevice) {
		this->device = device;
		this->physicalDevice = physicalDevice;
	}

	void VulkanSwapchain::cleanup() {
		vkDestroySwapchainKHR(device, swapchain, nullptr);
		vkDestroySurfaceKHR(instance, surface, nullptr);

		for (VkImageView imageView : imageViews) {
			vkDestroyImageView(device, imageView, nullptr);
		}
	}
}