#ifndef VULKAN_SRC_RENDERER_VULKAN_BOUNDIMAGE_H_
#define VULKAN_SRC_RENDERER_VULKAN_BOUNDIMAGE_H_

#include "renderer/vulkan/api/Image.h"
#include "renderer/vulkan/api/ImageView.h"
#include "renderer/vulkan/api/memory/BoundDeviceMemory.h"
#include "renderer/vulkan/api/structures/ImageViewCreateInfo.h"

class BoundImage {
 public:
  BoundImage() = default;
  BoundImage(Image image, BoundDeviceMemory memory);

  ImageView CreateView(const ImageViewCreateInfoBuilder& infoBuilder) const;

 private:
  Image image_;
  BoundDeviceMemory memory_;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_BOUNDIMAGE_H_
