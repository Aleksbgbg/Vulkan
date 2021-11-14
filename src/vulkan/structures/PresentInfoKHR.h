#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PRESENTINFOKHR_H
#define VULKAN_SRC_VULKAN_STRUCTURES_PRESENTINFOKHR_H

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(PresentInfoKhrBuilder, VkPresentInfoKHR,
                  VK_STRUCTURE_TYPE_PRESENT_INFO_KHR)
STRUCTURE_SETTER(WaitSemaphoreCount, u32, waitSemaphoreCount)
STRUCTURE_SETTER(PWaitSemaphores, const VkSemaphore*, pWaitSemaphores)
STRUCTURE_SETTER(SwapchainCount, u32, swapchainCount)
STRUCTURE_SETTER(PSwapchains, const VkSwapchainKHR*, pSwapchains)
STRUCTURE_SETTER(PImageIndices, const u32*, pImageIndices)
STRUCTURE_SETTER(PResults, VkResult*, pResults)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_PRESENTINFOKHR_H
