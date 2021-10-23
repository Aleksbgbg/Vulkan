#ifndef VULKAN_SRC_GAME_RENDERING_IMAGEWITHMEMORY_H
#define VULKAN_SRC_GAME_RENDERING_IMAGEWITHMEMORY_H

#include "memory/DeviceMemorySubAllocation.h"
#include "vulkan/Image.h"

struct ImageWithMemory {
  Image image;
  DeviceMemorySubAllocation memory;
};

#endif  // VULKAN_SRC_GAME_RENDERING_IMAGEWITHMEMORY_H
