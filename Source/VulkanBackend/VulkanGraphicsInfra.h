#pragma once

#include "VulkanBackendHeaders.h"
#include "Platform.h"

namespace VulkanBackend
{
	class VulkanGraphicsInfra
	{
	public:
		VulkanGraphicsInfra();

		void AdaptToWindow(Platform::IWindow* window);

	private:
		VkInstance	mInstance;
		VkDebugUtilsMessengerEXT mDebugMessenger;
		
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device;
	};
}