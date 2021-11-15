#ifndef VULKAN_SRC_VULKAN_SEMAPHORE_H
#define VULKAN_SRC_VULKAN_SEMAPHORE_H

#include <vulkan/vulkan.h>

#include "structures/SemaphoreCreateInfo.h"

class Semaphore {
 public:
  Semaphore();
  Semaphore(VkDevice device, const SemaphoreCreateInfoBuilder& infoBuilder);

  Semaphore(const Semaphore&) = delete;
  Semaphore(Semaphore&& other) noexcept;

  ~Semaphore();

  Semaphore& operator=(const Semaphore&) = delete;
  Semaphore& operator=(Semaphore&& other) noexcept;

  const VkSemaphore* Pointer() const;
  VkSemaphore Raw() const;

 private:
  VkDevice device;
  VkSemaphore semaphore;
};

#endif  // VULKAN_SRC_VULKAN_SEMAPHORE_H
