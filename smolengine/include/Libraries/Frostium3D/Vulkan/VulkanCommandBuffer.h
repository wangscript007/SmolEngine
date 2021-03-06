#pragma once
#ifndef FROSTIUM_OPENGL_IMPL
#include "Common/Core.h"
#include "Common/Common.h"
#include "Vulkan/Vulkan.h"

#include <mutex>

namespace Frostium
{
	class VulkanDevice;
	class VulkanCommandPool;

	struct CommandBufferStorage
	{
		VkCommandBuffer Buffer = VK_NULL_HANDLE;
		VkCommandPool   Pool = VK_NULL_HANDLE;
	};

	class VulkanCommandBuffer
	{
	public:

		VulkanCommandBuffer();

		bool                           Init(VulkanDevice* device);
		bool                           Create();
		bool                           Recrate();

		static void                    CreateCommandBuffer(CommandBufferStorage* data);
		static void                    ExecuteCommandBuffer(CommandBufferStorage* data);

		// Getters
		VkCommandBuffer                GetVkCommandBuffer() const;
		VkCommandPool                  GetVkCommandPool() const;
		size_t                         GetBufferSize() const;

	private:

		VkCommandPool                  m_CommandPool = VK_NULL_HANDLE;
		VulkanDevice*                  m_Device = nullptr;
		std::vector<VkCommandBuffer>   m_CommandBuffers;
	};
}
#endif