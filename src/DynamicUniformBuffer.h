#ifndef VULKAN_SRC_DYNAMICUNIFORMBUFFER_H_
#define VULKAN_SRC_DYNAMICUNIFORMBUFFER_H_

#include <cstring>

#include "BufferWithMemory.h"
#include "util/types.h"
#include "vulkan/DescriptorSet.h"

class DynamicUniformBufferInitializer {
 public:
  virtual ~DynamicUniformBufferInitializer() = default;

  virtual u32 PaddedSize(const u32 elementSize) const = 0;
  virtual BufferWithMemory CreateBuffer(const u32 elementPaddedSize) = 0;
};

template <typename Element>
class DynamicUniformBuffer {
 public:
  class Just {};

  DynamicUniformBuffer() = default;
  DynamicUniformBuffer(DynamicUniformBufferInitializer& initializer,
                       const DescriptorSet& descriptorSet)
      : elementPaddedSize(initializer.PaddedSize(sizeof(Element))),
        descriptorSet(&descriptorSet),
        bufferWithMemory(initializer.CreateBuffer(elementPaddedSize)),
        value() {}

  DescriptorSet::WriteDescriptorSet CreateWriteDescriptorSet(
      const u32 binding) const {
    return descriptorSet->CreateBufferWrite(
        bufferWithMemory.buffer, elementPaddedSize,
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, binding);
  }

  void Flush(const u32 objectIndex) const {
    const VkDeviceSize offset = objectIndex * elementPaddedSize;

    void* const mappedMemory =
        bufferWithMemory.memory.Map(offset, elementPaddedSize);
    std::memcpy(mappedMemory, &value, sizeof(Element));
    bufferWithMemory.memory.FlushMappedMemoryRange(offset, elementPaddedSize);
    bufferWithMemory.memory.Unmap();
  }

  u32 CalculateDynamicOffset(const u32 objectIndex) const {
    return objectIndex * elementPaddedSize;
  }

  Element& Value() {
    return value;
  }

 private:
  u32 elementPaddedSize;
  const DescriptorSet* descriptorSet;
  BufferWithMemory bufferWithMemory;
  Element value;
};

#endif  // VULKAN_SRC_DYNAMICUNIFORMBUFFER_H_
