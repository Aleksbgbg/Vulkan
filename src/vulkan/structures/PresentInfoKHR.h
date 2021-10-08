#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PRESENTINFOKHR_H
#define VULKAN_SRC_VULKAN_STRUCTURES_PRESENTINFOKHR_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER(PresentInfoKhrBuilder, VkPresentInfoKHR,
                  VK_STRUCTURE_TYPE_PRESENT_INFO_KHR)
STRUCTURE_SETTER(PresentInfoKhrBuilder, WaitSemaphoreCount, u32,
                 waitSemaphoreCount)
STRUCTURE_SETTER(PresentInfoKhrBuilder, PWaitSemaphores, const VkSemaphore*,
                 pWaitSemaphores)
STRUCTURE_SETTER(PresentInfoKhrBuilder, SwapchainCount, u32, swapchainCount)
STRUCTURE_SETTER(PresentInfoKhrBuilder, PSwapchains, const VkSwapchainKHR*,
                 pSwapchains)
STRUCTURE_SETTER(PresentInfoKhrBuilder, PImageIndices, const u32*,
                 pImageIndices)
STRUCTURE_SETTER(PresentInfoKhrBuilder, PResults, VkResult*, pResults)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_PRESENTINFOKHR_H
