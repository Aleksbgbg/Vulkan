#ifndef VULKAN_SRC_VULKAN_FENCE_H
#define VULKAN_SRC_VULKAN_FENCE_H

#include <vulkan/vulkan.h>

#include "structures/FenceCreateInfo.h"

class Fence {
 public:
  Fence();
  explicit Fence(const VkDevice device);
  Fence(const VkDevice device, const VkFenceCreateFlags createFlags);

  Fence(const Fence&) = delete;
  Fence(Fence&& other) noexcept;

  ~Fence();

  Fence& operator=(const Fence&) = delete;
  Fence& operator=(Fence&& other) noexcept;

  VkFence Raw() const;

  const Fence& Wait() const;
  const Fence& Reset() const;

 private:
  VkDevice device;
  VkFence fence;
};

#endif  // VULKAN_SRC_VULKAN_FENCE_H
