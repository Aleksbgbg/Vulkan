#ifndef VULKAN_SRC_RENDERER_VULKAN_IMAGEWITHMEMORY_H_
#define VULKAN_SRC_RENDERER_VULKAN_IMAGEWITHMEMORY_H_

#include "renderer/vulkan/api/Image.h"
#include "renderer/vulkan/api/memory/DeviceMemorySubAllocation.h"

struct ImageWithMemory {
  Image image;
  DeviceMemorySubAllocation memory;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_IMAGEWITHMEMORY_H_
