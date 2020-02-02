#include "VulkanModel.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "Core/Log.h"
#include <set>
#include <map>
#define GLM_ENABLE_EXPERIMENTAL	//for hash function
#include <glm/gtx/hash.hpp>
#include "VulkanUtilities.h"

namespace std {
	template<> struct hash<Engine::VulkanModel::Vertex> {
		size_t operator()(Engine::VulkanModel::Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.colors) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.uvs) << 1);
		}
	};
}


namespace Engine {

	void VulkanModel::loadFromFile(std::string modelPath, std::shared_ptr<VulkanDevice> vulkanDevice, VkQueue copyQueue, VkCommandPool commandPool) {
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		device = vulkanDevice->getDevice();

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str())) {
			throw std::runtime_error(warn + err);
		}

		LOG_INFO("{} vertices loaded", attrib.vertices.size());

		std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				Vertex vertex = {};

				vertex.pos = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				vertex.uvs = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};
				

				vertex.colors = { 1.0f, 1.0f, 1.0f };

				if (uniqueVertices.count(vertex) == 0) {
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}

				indices.push_back(uniqueVertices[vertex]);

			}
		}
		
		LOG_INFO("{} unique vertices", uniqueVertices.size());


		VkBuffer vertexStaging, indexStaging;
		VkDeviceMemory vertexBufferStagingMemory, indexBufferStagingMemory;

		VkDeviceSize verticesBufferSize = sizeof(vertices[0]) * vertices.size();

		//create vertex staging buffer
		VK_CHECK_RESULT(vulkanDevice->createBuffer(
			verticesBufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			vertexStaging,
			vertexBufferStagingMemory));

		//copy vertex data to staging buffer
		void* vertexData;
		vkMapMemory(vulkanDevice->getDevice(), vertexBufferStagingMemory, 0, verticesBufferSize, 0, &vertexData);
		memcpy(vertexData, vertices.data(), (size_t)verticesBufferSize);
		vkUnmapMemory(vulkanDevice->getDevice(), vertexBufferStagingMemory);

		//create vertex buffer
		vulkanDevice->createBuffer(
			verticesBufferSize,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			vertexBuffer,
			vertexBufferMemory);
		
		VkDeviceSize indexBufferSize = sizeof(indices[0]) * indices.size();

		//create index staging buffer
		VK_CHECK_RESULT(vulkanDevice->createBuffer(
			indexBufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			indexStaging,
			indexBufferStagingMemory));

		//copy index data to staging buffer
		void* indexData;
		vkMapMemory(vulkanDevice->getDevice(), indexBufferStagingMemory, 0, indexBufferSize, 0, &indexData);
		memcpy(indexData, indices.data(), (size_t)indexBufferSize);
		vkUnmapMemory(vulkanDevice->getDevice(), indexBufferStagingMemory);

		//create index buffer
		vulkanDevice->createBuffer(
			indexBufferSize,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			indexBuffer,
			indexBufferMemory);

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;			//TODO hantera med funktion istället? i VulkanDevice / utilities  ?
		VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer));


		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		VkBufferCopy bufferCopy{};
		VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &beginInfo));

			//copy from staging buffers
			bufferCopy.size = verticesBufferSize;
			vkCmdCopyBuffer(commandBuffer, vertexStaging, vertexBuffer, 1, &bufferCopy);
			bufferCopy.size = indexBufferSize;
			vkCmdCopyBuffer(commandBuffer, indexStaging, indexBuffer, 1, &bufferCopy);

		VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(copyQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(copyQueue);

		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

		//destroy buffers
		vkDestroyBuffer(device, vertexStaging, nullptr);
		vkFreeMemory(device, vertexBufferStagingMemory , nullptr);
		vkDestroyBuffer(device, indexStaging, nullptr);
		vkFreeMemory(device, indexBufferStagingMemory, nullptr);

	}

	void VulkanModel::cleanup() {
		vkDestroyBuffer(device, indexBuffer, nullptr);
		vkDestroyBuffer(device, vertexBuffer, nullptr);
		vkFreeMemory(device, indexBufferMemory, nullptr);
		vkFreeMemory(device, vertexBufferMemory, nullptr);
	}

}
