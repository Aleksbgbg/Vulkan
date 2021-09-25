#ifndef VULKAN_SRC_VULKAN_FENCE_H
#define VULKAN_SRC_VULKAN_FENCE_H

#include <vulkan/vulkan.h>

#include "lifetime_semantics.h"
#include "structures/FenceCreateInfo.h"

class Fence {
public:
  VULKAN_OBJECT_MOVABLE_ROOT(Fence, device, fence)

  explicit Fence(VkDevice device);
  Fence(VkDevice device, const VkFenceCreateFlags createFlags);

  Fence(const Fence&) = delete;

  ~Fence();

  Fence& operator=(const Fence&) = delete;

  VkFence Raw();

  Fence& Wait();
  Fence& Reset();

private:
  VkDevice device;
  VkFence fence = nullptr;
};

#endif // VULKAN_SRC_VULKAN_FENCE_H
