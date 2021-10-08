#ifndef VULKAN_SRC_VULKAN_FENCE_H
#define VULKAN_SRC_VULKAN_FENCE_H

#include <vulkan/vulkan.h>

#include "lifetime_semantics.h"
#include "structures/FenceCreateInfo.h"

class Fence {
 public:
  VULKAN_OBJECT_MOVABLE_ROOT(Fence, device, fence)

  Fence() = default;
  explicit Fence(const VkDevice device);
  Fence(const VkDevice device, const VkFenceCreateFlags createFlags);

  Fence(const Fence&) = delete;

  ~Fence();

  Fence& operator=(const Fence&) = delete;

  VkFence Raw() const;

  const Fence& Wait() const;
  const Fence& Reset() const;

 private:
  VkDevice device;
  VkFence fence = nullptr;
};

#endif  // VULKAN_SRC_VULKAN_FENCE_H
