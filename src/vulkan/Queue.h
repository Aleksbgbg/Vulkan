#ifndef VULKAN_SRC_VULKAN_QUEUE_H
#define VULKAN_SRC_VULKAN_QUEUE_H

#include <vulkan/vulkan.h>
#include <types.h>
#include "CommandPool.h"
#include "Swapchain.h"
#include "Semaphore.h"

class Queue {
public:
  Queue() = default;
  Queue(VkDevice device, VkQueue queue, const u32 familyIndex);

  CommandPool CreateCommandPool() const;
  CommandPool CreateCommandPool(const VkCommandPoolCreateFlags createFlags) const;

  VkResult Present(Swapchain& swapchain, const u32 imageIndex, const SynchronisationPack& synchronisation);

private:
  VkDevice device;
  VkQueue queue;
  u32 familyIndex;
};

#endif // VULKAN_SRC_VULKAN_QUEUE_H
