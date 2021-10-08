#ifndef VULKAN_SRC_VULKAN_IMAGEVIEW_H
#define VULKAN_SRC_VULKAN_IMAGEVIEW_H

#include <vulkan/vulkan.h>

#include "lifetime_semantics.h"
#include "structures/ImageViewCreateInfo.h"

class ImageView {
  friend class Swapchain;
  friend class DescriptorSet;

 public:
  VULKAN_OBJECT_MOVABLE_ROOT(ImageView, device, imageView)

  ImageView() = default;
  ImageView(VkDevice device, ImageViewCreateInfoBuilder& infoBuilder);

  ImageView(const ImageView&) = delete;

  ~ImageView();

  ImageView& operator=(const ImageView&) = delete;

 private:
  VkDevice device;
  VkImageView imageView = nullptr;
};

#endif  // VULKAN_SRC_VULKAN_IMAGEVIEW_H
