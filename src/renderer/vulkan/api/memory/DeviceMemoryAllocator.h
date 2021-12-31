#ifndef VULKAN_SRC_RENDERER_VULKAN_API_MEMORY_DEVICEMEMORYALLOCATOR_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_MEMORY_DEVICEMEMORYALLOCATOR_H_

#include <memory>
#include <vector>

#include "BoundDeviceMemory.h"
#include "memory/DeviceHeap.h"
#include "renderer/vulkan/api/Buffer.h"
#include "renderer/vulkan/api/DeviceMemory.h"
#include "renderer/vulkan/api/Image.h"
#include "renderer/vulkan/api/VirtualDevice.h"

class DeviceMemoryAllocator {
 public:
  DeviceMemoryAllocator() = default;
  DeviceMemoryAllocator(
      VirtualDevice* virtualDevice,
      const VkPhysicalDeviceMemoryProperties* const memoryProperties);

  BoundDeviceMemory BindMemory(const Buffer& buffer,
                               const VkMemoryPropertyFlags requiredProperties);
  BoundDeviceMemory BindMemory(const Image& image,
                               const VkMemoryPropertyFlags requiredProperties);

 private:
  BoundDeviceMemory ReserveMemoryBlock(
      const VkMemoryRequirements& memoryRequirements,
      const VkMemoryPropertyFlags requiredProperties);

 private:
  class HeapAllocator : public Allocator {
   public:
    HeapAllocator() = default;
    HeapAllocator(VirtualDevice* virtualDevice, u32 memoryTypeIndex);

    std::unique_ptr<MemoryObject> Allocate(VkDeviceSize size) override;

   private:
    VirtualDevice* virtualDevice_;
    u32 memoryTypeIndex_;
  };

  struct Heap {
    HeapAllocator allocator;
    DeviceHeap deviceHeap;
  };

  VirtualDevice* virtualDevice_;
  const VkPhysicalDeviceMemoryProperties* memoryProperties_;

  std::vector<Heap> heapsPerMemoryIndex_;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_MEMORY_DEVICEMEMORYALLOCATOR_H_
