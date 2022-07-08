#ifndef SRC_RENDERER_VULKAN_API_IMAGEVIEW_H_
#define SRC_RENDERER_VULKAN_API_IMAGEVIEW_H_

#include <vulkan/vulkan.h>

#include "renderer/vulkan/api/structures/ImageViewCreateInfo.h"

namespace vk {

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

}  // namespace vk

#endif  // SRC_RENDERER_VULKAN_API_IMAGEVIEW_H_
