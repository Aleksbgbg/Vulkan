#ifndef VULKAN_SRC_VULKAN_SYNCHRONISATIONPACK_H
#define VULKAN_SRC_VULKAN_SYNCHRONISATIONPACK_H

#include "Fence.h"
#include "Semaphore.h"

class SynchronisationPack {
 public:
  const Semaphore* GetWaitSemaphore() const;
  const Semaphore* GetSignalSemaphore() const;
  const Fence* GetSignalFence() const;

  SynchronisationPack& SetWaitSemaphore(const Semaphore* const semaphore);
  SynchronisationPack& SetSignalSemaphore(const Semaphore* const semaphore);
  SynchronisationPack& SetSignalFence(const Fence* const fence);

 private:
  const Semaphore* waitSemaphore;
  const Semaphore* signalSemaphore;
  const Fence* signalFence;
};

#endif  // VULKAN_SRC_VULKAN_SYNCHRONISATIONPACK_H
