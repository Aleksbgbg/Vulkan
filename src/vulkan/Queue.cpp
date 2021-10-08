#include "Queue.h"

#include <vulkan/structures/PresentInfoKHR.h>

#include "error.h"

Queue::Queue(VkDevice device, VkQueue queue, const u32 familyIndex)
    : device(device), queue(queue), familyIndex(familyIndex) {}

CommandPool Queue::CreateCommandPool() const {
  return CreateCommandPool(0);
}

CommandPool Queue::CreateCommandPool(
    const VkCommandPoolCreateFlags createFlags) const {
  return CommandPool(device, queue,
                     CommandPoolCreateInfoBuilder()
                         .SetFlags(createFlags)
                         .SetQueueFamilyIndex(familyIndex));
}

VkResult Queue::Present(Swapchain& swapchain, const u32 imageIndex,
                        const SynchronisationPack& synchronisation) {
  const VkResult status = vkQueuePresentKHR(
      queue,
      PresentInfoKhrBuilder()
          .SetWaitSemaphoreCount(1)
          .SetPWaitSemaphores(synchronisation.GetWaitSemaphore()->Pointer())
          .SetSwapchainCount(1)
          .SetPSwapchains(&swapchain.swapchain)
          .SetPImageIndices(&imageIndex)
          .Build());

  if (!((status == VK_ERROR_OUT_OF_DATE_KHR) ||
        (status == VK_SUBOPTIMAL_KHR))) {
    PROCEED_ON_VALID_RESULT(status)
  }

  return status;
}
