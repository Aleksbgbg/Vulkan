#ifndef VULKAN_SRC_RENDERER_VULKAN_API_QUEUE_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_QUEUE_H_

#include <vulkan/vulkan.h>

#include "CommandPool.h"
#include "Semaphore.h"
#include "Swapchain.h"
#include "util/types.h"

namespace vk {

class Queue {
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

}  // namespace vk

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_QUEUE_H_
