#pragma once

#include "VulkanBackendHeaders.h"
#include "Platform.h"
#include "VulkanResource.h"

namespace VulkanBackend
{
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

	class Swapchain
	{
	public:
		Swapchain(VkPhysicalDevice physicalDevice, VkDevice device, Platform::IWindow* window, VkSurfaceKHR surface, class ResourceManager* resourceManager);
		virtual ~Swapchain();

		VkSwapchainKHR	GetSwapchain() const { return swapchain; }
		VkFormat		GetFormat() const { return swapchainImageFormat; }
		VkExtent2D		GetExtent() const { return swapchainExtent; }
		u32				GetImage(u32 frameIndex) const { return swapchainImageIds[frameIndex]; }
		u32				GetFrameCount() const { return imageCount; }

	private:
		const u32 imageCount = 2;
		VkDevice device;
		VkSwapchainKHR swapchain;

		VkFormat swapchainImageFormat;
		VkExtent2D swapchainExtent;

		std::vector<u32> swapchainImageIds;
	};

	class VulkanGraphicsInfra
	{
	public:
		VulkanGraphicsInfra();
		virtual ~VulkanGraphicsInfra();

		void				AdaptToWindow(Platform::IWindow* window);
		void				DetachFromWindow(Platform::IWindow* window);

		VkInstance			GetVkInstance() const { return mInstance; }
		VkPhysicalDevice	GetPhysicalDevice() const { return physicalDevice; }
		VkDevice			GetDevice() const { return device; }
		VkQueue				GetGraphicsQueue() const { return graphicsQueue; }
		VkQueue				GetPresentQueue() const { return presentQueue; }

		Swapchain*			GetSwapchain(Platform::IWindow* window) const;
		QueueFamilyIndices	GetQueueFamilyIndices(Platform::IWindow* window) const;
		ResourceManager*	GetResourceManager() const { return mResourceManager.get(); }

	private:
		VkInstance	mInstance;
		VkDebugUtilsMessengerEXT mDebugMessenger;
		
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		VkDevice device;

		VkQueue graphicsQueue;
		VkQueue presentQueue;

		std::unique_ptr<ResourceManager> mResourceManager;

		std::map<Platform::IWindow*, QueueFamilyIndices> mQueueFamilyIndices;
		std::map<Platform::IWindow*, std::unique_ptr<Swapchain>> mSwapchains;
		std::map<Platform::IWindow*, VkSurfaceKHR> mSurfaces;
	};
}