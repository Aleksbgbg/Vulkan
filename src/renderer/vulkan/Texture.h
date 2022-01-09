#ifndef VULKAN_SRC_RENDERER_VULKAN_TEXTURE_H_
#define VULKAN_SRC_RENDERER_VULKAN_TEXTURE_H_

#include "BoundImage.h"
#include "renderer/vulkan/api/ImageView.h"

struct Texture {
  BoundImage image;
  vk::ImageView view;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_TEXTURE_H_
