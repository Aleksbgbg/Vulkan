#ifndef VULKAN_SRC_RENDERER_VULKAN_BOUNDBUFFER_H_
#define VULKAN_SRC_RENDERER_VULKAN_BOUNDBUFFER_H_

#include "core/types.h"
#include "renderer/vulkan/api/Buffer.h"
#include "renderer/vulkan/api/DescriptorSet.h"
#include "renderer/vulkan/api/memory/BoundDeviceMemory.h"

class BoundBuffer {
 public:
  BoundBuffer() = default;
  BoundBuffer(vk::Buffer buffer, vk::BoundDeviceMemory memory);

  const vk::Buffer& RawBuffer() const;

  vk::DescriptorSet::WriteDescriptorSet WriteBuffer(
      const vk::DescriptorSet& descriptorSet, VkDescriptorType descriptorType,
      u32 binding) const;

  void* Map() const;
  void Unmap() const;

  void MapCopy(const void* data) const;
  void MapCopy(const void* data, VkDeviceSize size) const;

 private:
  vk::Buffer buffer_;
  vk::BoundDeviceMemory memory_;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_BOUNDBUFFER_H_
