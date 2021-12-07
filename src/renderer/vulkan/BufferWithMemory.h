#ifndef VULKAN_SRC_RENDERER_VULKAN_BUFFERWITHMEMORY_H_
#define VULKAN_SRC_RENDERER_VULKAN_BUFFERWITHMEMORY_H_

#include "memory/DeviceMemorySubAllocation.h"
#include "renderer/vulkan/api/Buffer.h"

struct BufferWithMemory {
  Buffer buffer;
  DeviceMemorySubAllocation memory;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_BUFFERWITHMEMORY_H_
