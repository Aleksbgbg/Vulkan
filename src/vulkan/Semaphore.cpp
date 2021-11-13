#include "Semaphore.h"

#include "error.h"

Semaphore::Semaphore(VkDevice device, SemaphoreCreateInfoBuilder& infoBuilder)
    : device(device) {
  PROCEED_ON_VALID_RESULT(
      vkCreateSemaphore(device, infoBuilder.Build(), nullptr, &semaphore));
}

Semaphore::~Semaphore() {
  if (semaphore != nullptr) {
    vkDestroySemaphore(device, semaphore, nullptr);
  }
}

VkSemaphore* Semaphore::Pointer() {
  return &semaphore;
}

VkSemaphore Semaphore::Raw() {
  return semaphore;
}
