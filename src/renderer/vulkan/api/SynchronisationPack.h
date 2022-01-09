#ifndef VULKAN_SRC_RENDERER_VULKAN_API_SYNCHRONISATIONPACK_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_SYNCHRONISATIONPACK_H_

#include "Fence.h"
#include "Semaphore.h"

namespace vk {

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

}  // namespace vk

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_SYNCHRONISATIONPACK_H_
