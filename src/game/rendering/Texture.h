#ifndef VULKAN_SRC_GAME_RENDERING_TEXTURE_H_
#define VULKAN_SRC_GAME_RENDERING_TEXTURE_H_

#include "ImageWithMemory.h"
#include "vulkan/ImageView.h"

struct Texture {
  ImageWithMemory image;
  ImageView view;
};

#endif  // VULKAN_SRC_GAME_RENDERING_TEXTURE_H_
