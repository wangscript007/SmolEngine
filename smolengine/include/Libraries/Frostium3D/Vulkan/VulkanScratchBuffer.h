#pragma once
#ifndef FROSTIUM_OPENGL_IMPL
#include "Vulkan/VulkanBuffer.h"

namespace Frostium
{
	class VulkanScratchBuffer : public VulkanBuffer
	{
	public:

		VulkanScratchBuffer() = default;
		~VulkanScratchBuffer();

		void Create(size_t size);
		// Getters
		uint64_t GetDeviceAddress();

	private:

		uint64_t m_DeviceAddress = 0;
	};
}
#endif