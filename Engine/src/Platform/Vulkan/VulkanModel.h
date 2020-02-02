#pragma once
#include "vulkan/vulkan.h"
#include <string>
#include "VulkanDevice.h"
#include <memory>
#include <glm/glm.hpp>

namespace Engine {
	
	

	class VulkanModel {
	public:
		struct Vertex {
			glm::vec3 pos;
			glm::vec3 colors;
			glm::vec2 uvs;

			bool operator==(const Vertex& other) const {
				return pos == other.pos && colors == other.colors && uvs == other.uvs;
			}

		};
		void loadFromFile(std::string filepath, std::shared_ptr<VulkanDevice> vulkanDevice, VkQueue copyQueue, VkCommandPool commandPool);
		void cleanup();
		VkBuffer indexBuffer;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		VkDeviceMemory indexBufferMemory;

		std::vector<uint32_t> indices;
	private:
		std::vector<Vertex> vertices;
		VkDevice device;

	};


}