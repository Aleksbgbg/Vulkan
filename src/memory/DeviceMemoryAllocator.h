#ifndef VULKAN_SRC_MEMORY_DEVICEMEMORYALLOCATOR_H
#define VULKAN_SRC_MEMORY_DEVICEMEMORYALLOCATOR_H

#include <memory>
#include <vector>

#include "vulkan/Buffer.h"
#include "vulkan/DeviceMemory.h"
#include "vulkan/Image.h"
#include "vulkan/VirtualDevice.h"
#include "DeviceHeap.h"

struct ReservedMemory {
  friend class DeviceMemoryAllocator;

  DeviceMemory* memory;
  VkDeviceSize offset;

private:
  ReservedBlock reservedBlock;
};

class DeviceMemoryAllocator {
public:
  DeviceMemoryAllocator() = default;
  DeviceMemoryAllocator(VirtualDevice* virtualDevice, const VkPhysicalDeviceMemoryProperties* const memoryProperties);

  ReservedMemory BindMemory(const Buffer& buffer, const VkMemoryPropertyFlags requiredProperties);
  ReservedMemory BindMemory(const Image& image, const VkMemoryPropertyFlags requiredProperties);

private:
  ReservedMemory ReserveMemoryBlock(
      const VkMemoryRequirements& memoryRequirements, const VkMemoryPropertyFlags requiredProperties);

private:
  class HeapAllocator : public Allocator {
  public:
    HeapAllocator() = default;
    HeapAllocator(VirtualDevice* virtualDevice, u32 memoryTypeIndex);

    std::unique_ptr<MemoryObject> Allocate(VkDeviceSize size) override;

  private:
    VirtualDevice* virtualDevice;
    u32 memoryTypeIndex;
  };

  struct Heap {
    HeapAllocator allocator;
    DeviceHeap deviceHeap;
  };

  VirtualDevice* virtualDevice;
  const VkPhysicalDeviceMemoryProperties* memoryProperties;

  std::vector<Heap> heapsPerMemoryIndex;
};

#endif // VULKAN_SRC_MEMORY_DEVICEMEMORYALLOCATOR_H
