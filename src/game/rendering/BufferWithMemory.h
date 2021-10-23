#ifndef VULKAN_SRC_GAME_RENDERING_BUFFERWITHMEMORY_H
#define VULKAN_SRC_GAME_RENDERING_BUFFERWITHMEMORY_H

#include "memory/DeviceMemorySubAllocation.h"
#include "vulkan/Buffer.h"

struct BufferWithMemory {
  Buffer buffer;
  DeviceMemorySubAllocation memory;
};

#endif  // VULKAN_SRC_GAME_RENDERING_BUFFERWITHMEMORY_H
