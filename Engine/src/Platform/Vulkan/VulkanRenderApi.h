#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <optional>
#include "Renderer/RenderApi.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <array>
#include "VulkanDevice.h"
#include <memory>
#include "VulkanSwapchain.h"
#include "VulkanTexture.h"
#include "VulkanModel.h"

namespace Engine {

	class VulkanRenderApi : public RenderApi {
	public:
		~VulkanRenderApi();

		virtual void init() override;
		virtual void draw() override;

	private:
		const int MAX_FRAMES_IN_FLIGHT = 2;

		std::shared_ptr<VulkanDevice> vulkanDevice;
		std::unique_ptr<VulkanSwapchain> vulkanSwapchain;
		VulkanTexture texture;
		VulkanModel model;
		
		VkPhysicalDevice physicalDevice;
		VkDevice device;
		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		std::vector<VkFramebuffer> swapChainFramebuffers;
		std::vector<VkCommandBuffer> commandBuffers;
		VkRenderPass renderPass;
		VkDescriptorSetLayout descriptorSetLayout;
		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;
		VkCommandPool commandPool;
		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;
		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;

		VkImage depthImage;
		VkDeviceMemory depthImageMemory;
		VkImageView depthImageView;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;
		size_t currentFrame = 0;
		bool framebufferResized = false;

		void createInstance();
		VkPhysicalDevice pickPhysicalDevice();
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		bool checkValidationLayerSupport();
		void setupDebugMessenger();
		bool isDeviceSuitable(VkPhysicalDevice device);

		void createRenderPass();
		void createGraphicsPipeline();
		void createFrameBuffers();
		void createCommandPool();
		void loadModel();
		void createUniformBuffers();
		void createCommandBuffers();
		void createSyncObjects();
		void createDescriptorSetLayout();
		void createDescriptorPool();
		void createDescriptorSets();
		void createDepthResources();
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		VkCommandBuffer beginSingleTimeCommands();
		void endSingleTimeCommands(VkCommandBuffer commandBuffer);
		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat findDepthFormat();
		bool hasStencilComponent(VkFormat format);
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		void updateUniformBuffer(uint32_t imageIndex);

		std::vector<const char*> getRequiredExtensions();
		
		const std::string MODEL_PATH = ASSETS_PATH + std::string("models/chalet.obj");
		const std::string TEXTURE_PATH = ASSETS_PATH + std::string("textures/chalet.jpg");

		struct QueueFamilyIndices {
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;


			bool isComplete() {
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		};

		struct SwapChainSupportDetails {
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};
				
		struct UniformBufferObject {
			alignas(16) glm::mat4 model;
			alignas(16) glm::mat4 view;
			alignas(16) glm::mat4 proj;
		};


		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		VkShaderModule createShaderModule(const std::vector<char>& code);


		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugUtilsMessengerEXT* pDebugMessenger);

		void DestroyDebugUtilsMessengerEXT(VkInstance instance,
			VkDebugUtilsMessengerEXT debugMessenger,
			const VkAllocationCallbacks* pAllocator);



		#ifdef DEBUG
			const bool enableValidationLayers = true;
		#else
			const bool enableValidationLayers = false;
		#endif

	};



}
