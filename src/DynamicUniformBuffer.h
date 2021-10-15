#ifndef VULKAN_SRC_DYNAMICUNIFORMBUFFER_H
#define VULKAN_SRC_DYNAMICUNIFORMBUFFER_H

#include "memory/Alignment.h"
#include "memory/DeviceMemoryAllocator.h"
#include "types.h"
#include "vulkan/DescriptorSet.h"
#include "vulkan/structures/default.h"

template <typename Element>
class DynamicUniformBuffer {
 public:
  DynamicUniformBuffer() = default;
  DynamicUniformBuffer(const u32 objectCount,
                       const VkPhysicalDeviceLimits& limits,
                       const DescriptorPool& descriptorPool,
                       const DescriptorSetLayout& descriptorSetLayout,
                       const VirtualDevice& virtualDevice,
                       DeviceMemoryAllocator& allocator)
      : elementPaddedSize(Align(sizeof(Element), limits.nonCoherentAtomSize)),
        descriptorSet(
            descriptorPool.AllocateDescriptorSet(descriptorSetLayout)),
        buffer(virtualDevice.CreateBuffer(
            BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
                .SetSize(elementPaddedSize * objectCount)
                .SetUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT))),
        memory(
            allocator.BindMemory(buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)),
        value() {}

  void CreateWriteDescriptorSet(
      DescriptorSet::WriteDescriptorSet& write) const {
    descriptorSet.CreateBufferWrite(buffer, elementPaddedSize,
                                    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                                    write);
  }

  void Flush(const u32 objectIndex) const {
    const VkDeviceSize offset = objectIndex * elementPaddedSize;

    void* const mappedMemory = memory.Map(offset, elementPaddedSize);
    std::memcpy(mappedMemory, &value, sizeof(Element));
    memory.FlushMappedMemoryRange(offset, elementPaddedSize);
    memory.Unmap();
  }

  void BindDescriptorSets(const CommandBuffer& commandBuffer,
                          const PipelineLayout& pipelineLayout, const u32 set,
                          const u32 objectIndex) const {
    const u32 offset = objectIndex * elementPaddedSize;
    commandBuffer.CmdBindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS,
                                        pipelineLayout, set, 1, descriptorSet,
                                        1, &offset);
  }

  Element& Value() {
    return value;
  }

 private:
  u32 elementPaddedSize;
  DescriptorSet descriptorSet;
  Buffer buffer;
  DeviceMemorySubAllocation memory;
  Element value;
};

#endif  // VULKAN_SRC_DYNAMICUNIFORMBUFFER_H
