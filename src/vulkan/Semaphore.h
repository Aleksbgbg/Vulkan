#ifndef VULKAN_SRC_VULKAN_SEMAPHORE_H
#define VULKAN_SRC_VULKAN_SEMAPHORE_H

#include <vulkan/vulkan.h>

#include "lifetime_semantics.h"
#include "structures/SemaphoreCreateInfo.h"

class Semaphore {
 public:
  VULKAN_OBJECT_MOVABLE_ROOT(Semaphore, device, semaphore)

  Semaphore() = default;
  Semaphore(VkDevice device, SemaphoreCreateInfoBuilder& infoBuilder);

  Semaphore(const Semaphore&) = delete;

  ~Semaphore();

  Semaphore& operator=(const Semaphore&) = delete;

  void Wait();

  VkSemaphore* Pointer();
  VkSemaphore Raw();

 private:
  VkDevice device;
  VkSemaphore semaphore = nullptr;
};

#endif  // VULKAN_SRC_VULKAN_SEMAPHORE_H
