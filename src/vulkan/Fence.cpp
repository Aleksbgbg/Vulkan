#include "Fence.h"

#include "error.h"

Fence::Fence() : fence(nullptr) {}

Fence::Fence(const VkDevice device) : Fence(device, 0) {}

Fence::Fence(const VkDevice device, const VkFenceCreateFlags createFlags)
    : device(device) {
  PROCEED_ON_VALID_RESULT(vkCreateFence(
      device, FenceCreateInfoBuilder().SetFlags(createFlags).Build(), nullptr,
      &fence));
}

Fence::Fence(Fence&& other) noexcept : device(other.device), fence(other.fence) {
  other.fence = nullptr;
}

Fence::~Fence() {
  if (fence != nullptr) {
    vkDestroyFence(device, fence, nullptr);
  }
}

Fence& Fence::operator=(Fence&& other) noexcept {
  std::swap(device, other.device);
  std::swap(fence, other.fence);
  return *this;
}

VkFence Fence::Raw() const {
  return fence;
}

const Fence& Fence::Wait() const {
  PROCEED_ON_VALID_RESULT(
      vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX))
  return *this;
}

const Fence& Fence::Reset() const {
  PROCEED_ON_VALID_RESULT(vkResetFences(device, 1, &fence))
  return *this;
}
