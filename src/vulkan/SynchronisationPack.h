#ifndef VULKAN_SRC_VULKAN_SYNCHRONISATIONPACK_H
#define VULKAN_SRC_VULKAN_SYNCHRONISATIONPACK_H

#include "Fence.h"
#include "Semaphore.h"

class SynchronisationPack {
 public:
  Semaphore* GetWaitSemaphore() const;
  Semaphore* GetSignalSemaphore() const;
  Fence* GetSignalFence() const;

  SynchronisationPack& SetWaitSemaphore(Semaphore* const semaphore);
  SynchronisationPack& SetSignalSemaphore(Semaphore* const semaphore);
  SynchronisationPack& SetSignalFence(Fence* const fence);

 private:
  Semaphore* waitSemaphore;
  Semaphore* signalSemaphore;
  Fence* signalFence;
};

#endif  // VULKAN_SRC_VULKAN_SYNCHRONISATIONPACK_H
