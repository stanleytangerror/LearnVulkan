#include "VulkanResource.h"

namespace VulkanBackend
{
	static uint32_t findMemoryType(VkPhysicalDeviceMemoryProperties memProperties, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

	ResourceManager::ResourceManager(VkPhysicalDevice physicalDevice, VkDevice device)
		: physicalDevice(physicalDevice), device(device)
	{
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
	}

	i32 ResourceManager::PossessResourceWithOwnership(VkImage image, const ImageResourceInfo& info)
	{
		i32 id = resourceAllocator++;

		mImageResources[id] = std::make_unique<ImageResource>(image, VK_NULL_HANDLE, info);

		return id;
	}

	i32 ResourceManager::CreateResource(const ImageResourceInfo& imageInfo, const DeviceMemoryInfo& memoryInfo)
	{
		VkImageCreateInfo imageCreateInfo{};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		std::memcpy(&(imageCreateInfo.imageType), &imageInfo, sizeof(imageInfo));

		VkImage image;
		if (vkCreateImage(device, &imageCreateInfo, nullptr, &image) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(device, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memProperties, memRequirements.memoryTypeBits, memoryInfo.propertyFlags);

		VkDeviceMemory imageMemory;
		if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate image memory!");
		}

		vkBindImageMemory(device, image, imageMemory, 0);

		i32 id = resourceAllocator++;
		mImageResources[id] = std::make_unique<ImageResource>(image, imageMemory, imageInfo);

		return id;
	}

	VkImageView ResourceManager::CreateImageView(i32 resourceId, const ImageViewInfo& info)
	{
		auto it = mImageResources.find(resourceId);
		if (it == mImageResources.end())
		{
			throw std::runtime_error("failed to find image resource!");
		}

		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = it->second->GetResource();
		viewInfo.viewType = info.viewType;
		viewInfo.format = info.format;
		viewInfo.subresourceRange = info.subresourceRange;

		VkImageView imageView;
		if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture image view!");
		}

		return imageView;
	}

	ImageResource::ImageResource(VkImage resource, VkDeviceMemory memory, ImageResourceInfo info)
		: resource(resource)
		, memory(memory)
		, info(info) 
	{

	}

	ImageResource::~ImageResource()
	{

	}
}