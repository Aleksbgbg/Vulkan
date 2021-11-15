#ifndef VULKAN_SRC_VULKAN_QUEUE_H
#define VULKAN_SRC_VULKAN_QUEUE_H

#include <vulkan/vulkan.h>

#include "CommandPool.h"
#include "Semaphore.h"
#include "Swapchain.h"
#include "util/types.h"

class Queue {
  friend class ImGuiInstance;

 public:
  Queue() = default;
  Queue(VkDevice device, VkQueue queue, const u32 familyIndex);

  CommandPool CreateCommandPool() const;
  CommandPool CreateCommandPool(
      const VkCommandPoolCreateFlags createFlags) const;

  VkResult Present(const Swapchain& swapchain, const u32 imageIndex,
                   const SynchronisationPack& synchronisation) const;

 private:
  VkDevice device;
  VkQueue queue;
  u32 familyIndex;
};

#endif  // VULKAN_SRC_VULKAN_QUEUE_H
