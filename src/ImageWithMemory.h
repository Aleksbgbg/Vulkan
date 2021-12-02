#ifndef VULKAN_SRC_IMAGEWITHMEMORY_H_
#define VULKAN_SRC_IMAGEWITHMEMORY_H_

#include "memory/DeviceMemorySubAllocation.h"
#include "vulkan/Image.h"

struct ImageWithMemory {
  Image image;
  DeviceMemorySubAllocation memory;
};

#endif  // VULKAN_SRC_IMAGEWITHMEMORY_H_
