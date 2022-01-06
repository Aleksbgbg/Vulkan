#ifndef VULKAN_SRC_RENDERER_VULKAN_BOUNDBUFFER_H_
#define VULKAN_SRC_RENDERER_VULKAN_BOUNDBUFFER_H_

#include "renderer/vulkan/api/Buffer.h"
#include "renderer/vulkan/api/DescriptorSet.h"
#include "renderer/vulkan/api/memory/BoundDeviceMemory.h"
#include "util/types.h"

class BoundBuffer {
 public:
  BoundBuffer() = default;
  BoundBuffer(Buffer buffer, BoundDeviceMemory memory);

  const Buffer& RawBuffer() const;

  DescriptorSet::WriteDescriptorSet WriteBuffer(
      const DescriptorSet& descriptorSet, VkDescriptorType descriptorType,
      u32 binding) const;

  void* Map() const;
  void Unmap() const;

  void MapCopy(const void* data) const;
  void MapCopy(const void* data, VkDeviceSize size) const;

 private:
  Buffer buffer_;
  BoundDeviceMemory memory_;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_BOUNDBUFFER_H_
