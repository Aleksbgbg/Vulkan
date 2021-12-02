#ifndef VULKAN_SRC_GAME_RENDERING_RESOURCES_TEXTUREBINDER_H_
#define VULKAN_SRC_GAME_RENDERING_RESOURCES_TEXTUREBINDER_H_

#include <vulkan/vulkan.h>

#include "util/types.h"
#include "vulkan/ImageView.h"

class TextureBinder {
 public:
  virtual ~TextureBinder() = default;

  virtual void BindTexture(const ImageView& textureView, const u32 binding) = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERING_RESOURCES_TEXTUREBINDER_H_
