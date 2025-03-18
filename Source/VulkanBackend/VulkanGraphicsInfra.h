#pragma once

#include "VulkanBackendHeaders.h"
#include "Platform.h"

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

	struct ImageResourceInfo 
	{
		VkImageType              imageType;
		VkFormat                 format;
		VkExtent3D               extent;
		uint32_t                 mipLevels;
		uint32_t                 arrayLayers;
		VkSampleCountFlagBits    samples;
		VkImageTiling            tiling;
		VkImageUsageFlags        usage;
		VkSharingMode            sharingMode;
		uint32_t                 queueFamilyIndexCount;
		const uint32_t*			 pQueueFamilyIndices;
		VkImageLayout            initialLayout;
	};

	struct DeviceMemoryInfo
	{
		VkMemoryPropertyFlags		propertyFlags;
	};

	struct ImageViewInfo
	{
		VkImageViewType            viewType;
		VkFormat                   format;
		VkImageSubresourceRange    subresourceRange;
	};

	class ImageResource
	{
	public:
		ImageResource(VkImage resource, VkDeviceMemory memory, ImageResourceInfo info) 
			: resource(resource), memory(memory), info(info) {}

		virtual ~ImageResource();

		VkImage GetResource() const { return resource; }

	private:
		VkImage resource;
		VkDeviceMemory memory;
		ImageResourceInfo info;
	};

	class ResourceManager
	{
	public:
		ResourceManager(VkPhysicalDevice physicalDevice, VkDevice device);
		i32 PossessResourceWithOwnership(VkImage image, const ImageResourceInfo& info);
		i32 CreateResource(const ImageResourceInfo& imageInfo, const DeviceMemoryInfo& memoryInfo);

		VkImageView CreateImageView(i32 resourceId, const ImageViewInfo& info);

	private:
		VkPhysicalDevice physicalDevice;
		VkPhysicalDeviceMemoryProperties memProperties;
		VkDevice device;
		i32 resourceAllocator = 0;
		std::map<i32, std::unique_ptr<ImageResource>> mImageResources;
		std::map<i32, std::vector<VkImageView>> mImageViews;
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