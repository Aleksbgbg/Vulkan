#ifndef VULKAN_SRC_VULKAN_IMAGEVIEW_H
#define VULKAN_SRC_VULKAN_IMAGEVIEW_H

#include <vulkan/vulkan.h>

#include "structures/ImageViewCreateInfo.h"

class ImageView {
  friend class Swapchain;
  friend class DescriptorSet;

 public:
  ImageView();
  ImageView(VkDevice device, const ImageViewCreateInfoBuilder& infoBuilder);

  ImageView(const ImageView&) = delete;
  ImageView(ImageView&& other) noexcept;

  ~ImageView();

  ImageView& operator=(const ImageView&) = delete;
  ImageView& operator=(ImageView&& other) noexcept;

 private:
  VkDevice device;
  VkImageView imageView;
};

#endif  // VULKAN_SRC_VULKAN_IMAGEVIEW_H
