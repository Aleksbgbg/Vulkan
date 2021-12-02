#ifndef VULKAN_SRC_BUFFERWITHMEMORY_H_
#define VULKAN_SRC_BUFFERWITHMEMORY_H_

#include "memory/DeviceMemorySubAllocation.h"
#include "vulkan/Buffer.h"

struct BufferWithMemory {
  Buffer buffer;
  DeviceMemorySubAllocation memory;
};

#endif  // VULKAN_SRC_BUFFERWITHMEMORY_H_
