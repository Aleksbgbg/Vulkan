#ifndef VULKAN_SRC_RENDERER_VULKAN_API_IMAGEVIEW_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_IMAGEVIEW_H_

#include <vulkan/vulkan.h>

#include "renderer/vulkan/api/structures/ImageViewCreateInfo.h"

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

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_IMAGEVIEW_H_