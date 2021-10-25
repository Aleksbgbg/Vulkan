#ifndef VULKAN_SRC_GAME_RENDERING_TEXTURE_H
#define VULKAN_SRC_GAME_RENDERING_TEXTURE_H

#include "ImageWithMemory.h"
#include "vulkan/ImageView.h"

struct Texture {
  ImageWithMemory image;
  ImageView view;
};

#endif  // VULKAN_SRC_GAME_RENDERING_TEXTURE_H
