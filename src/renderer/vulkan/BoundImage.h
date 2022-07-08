#ifndef SRC_RENDERER_VULKAN_BOUNDIMAGE_H_
#define SRC_RENDERER_VULKAN_BOUNDIMAGE_H_

#include "renderer/vulkan/api/Image.h"
#include "renderer/vulkan/api/ImageView.h"
#include "renderer/vulkan/api/memory/BoundDeviceMemory.h"
#include "renderer/vulkan/api/structures/ImageViewCreateInfo.h"

class BoundImage {
 public:
  BoundImage() = default;
  BoundImage(vk::Image image, vk::BoundDeviceMemory memory);

  vk::ImageView CreateView(const ImageViewCreateInfoBuilder& infoBuilder) const;

 private:
  vk::Image image_;
  vk::BoundDeviceMemory memory_;
};

#endif  // SRC_RENDERER_VULKAN_BOUNDIMAGE_H_
