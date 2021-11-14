#include "Semaphore.h"

#include "error.h"

Semaphore::Semaphore() : semaphore(nullptr) {}

Semaphore::Semaphore(VkDevice device,
                     const SemaphoreCreateInfoBuilder& infoBuilder)
    : device(device) {
  PROCEED_ON_VALID_RESULT(
      vkCreateSemaphore(device, infoBuilder.Build(), nullptr, &semaphore));
}

Semaphore::Semaphore(Semaphore&& other) noexcept
    : device(other.device), semaphore(other.semaphore) {
  other.semaphore = nullptr;
}

Semaphore::~Semaphore() {
  if (semaphore != nullptr) {
    vkDestroySemaphore(device, semaphore, nullptr);
  }
}

Semaphore& Semaphore::operator=(Semaphore&& other) noexcept {
  std::swap(device, other.device);
  std::swap(semaphore, other.semaphore);
  return *this;
}

VkSemaphore* Semaphore::Pointer() {
  return &semaphore;
}

VkSemaphore Semaphore::Raw() {
  return semaphore;
}
