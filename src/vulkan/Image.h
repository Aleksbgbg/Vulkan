#ifndef VULKAN_SRC_VULKAN_IMAGE_H_
#define VULKAN_SRC_VULKAN_IMAGE_H_

#include <vulkan/vulkan.h>

#include "ImageView.h"
#include "structures/ImageCreateInfo.h"

class Image {
  friend class CommandBuffer;
  friend class DeviceMemory;

 public:
  Image();
  Image(VkDevice device, const ImageCreateInfoBuilder& infoBuilder);

  Image(const Image&) = delete;
  Image(Image&& other) noexcept;

  ~Image();

  Image& operator=(const Image&) = delete;
  Image& operator=(Image&& other) noexcept;

  VkMemoryRequirements GetMemoryRequirements() const;

  ImageView CreateView(ImageViewCreateInfoBuilder& infoBuilder);

 private:
  VkDevice device;
  VkImage image;
  VkImageCreateInfo createInfo;
};

#endif  // VULKAN_SRC_VULKAN_IMAGE_H_
