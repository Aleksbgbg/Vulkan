#ifndef VULKAN_SRC_GAME_RENDERING_RESOURCES_BUFFERBINDER_H_
#define VULKAN_SRC_GAME_RENDERING_RESOURCES_BUFFERBINDER_H_

#include <vulkan/vulkan.h>

#include "util/types.h"
#include "vulkan/Buffer.h"

class BufferBinder {
 public:
  virtual ~BufferBinder() = default;

  virtual void BindBuffer(const Buffer& buffer, const VkDeviceSize range,
                          const VkDescriptorType descriptorType,
                          const u32 binding) = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERING_RESOURCES_BUFFERBINDER_H_
