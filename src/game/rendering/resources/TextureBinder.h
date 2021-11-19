#ifndef VULKAN_SRC_GAME_RENDERING_RESOURCES_TEXTUREBINDER_H
#define VULKAN_SRC_GAME_RENDERING_RESOURCES_TEXTUREBINDER_H

#include <vulkan/vulkan.h>

#include "util/types.h"
#include "vulkan/ImageView.h"

class TextureBinder {
 public:
  virtual ~TextureBinder() = default;

  virtual void BindTexture(const ImageView& textureView, const u32 binding) = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERING_RESOURCES_TEXTUREBINDER_H
