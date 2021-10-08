#ifndef VULKAN_SRC_VULKAN_IMAGE_H
#define VULKAN_SRC_VULKAN_IMAGE_H

#include <vulkan/vulkan.h>

#include "ImageView.h"
#include "lifetime_semantics.h"
#include "structures/ImageCreateInfo.h"

class Image {
  friend class DeviceMemory;
  friend class CommandBuffer;

 public:
  Image() = default;
  Image(VkDevice device, VkPhysicalDeviceMemoryProperties* memoryProperties,
        ImageCreateInfoBuilder& infoBuilder);

  Image(const Image&) = delete;
  Image(Image&& other) noexcept
      : device(other.device),
        memoryProperties(other.memoryProperties),
        image(other.image),
        createInfo(other.createInfo) {
    other.image = nullptr;
  }

  ~Image();

  Image& operator=(const Image&) = delete;
  Image& operator=(Image&& other) noexcept {
    device = other.device;
    memoryProperties = other.memoryProperties;
    std::swap(image, other.image);
    createInfo = other.createInfo;
    return *this;
  }

  VkMemoryRequirements GetMemoryRequirements() const;

  DeviceMemory AllocateAndBindMemory(
      const VkMemoryPropertyFlags requiredProperties) const;
  ImageView CreateView(ImageViewCreateInfoBuilder& infoBuilder);

 private:
  VkDevice device;
  VkPhysicalDeviceMemoryProperties* memoryProperties;
  VkImage image = nullptr;
  VkImageCreateInfo createInfo;
};

#endif  // VULKAN_SRC_VULKAN_IMAGE_H
