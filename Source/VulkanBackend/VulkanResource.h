#pragma once

#include "VulkanBackendHeaders.h"

namespace VulkanBackend
{
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
		const uint32_t* pQueueFamilyIndices;
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
		ImageResource(VkImage resource, VkDeviceMemory memory, ImageResourceInfo info);
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
}