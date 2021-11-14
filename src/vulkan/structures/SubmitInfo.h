#ifndef VULKAN_SRC_VULKAN_STRUCTURES_SUBMITINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_SUBMITINFO_H

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(SubmitInfoBuilder, VkSubmitInfo,
                  VK_STRUCTURE_TYPE_SUBMIT_INFO)
STRUCTURE_SETTER(WaitSemaphoreCount, const u32, waitSemaphoreCount)
STRUCTURE_SETTER(PWaitSemaphores, const VkSemaphore*, pWaitSemaphores)
STRUCTURE_SETTER(PWaitDstStageMask, const VkPipelineStageFlags*,
                 pWaitDstStageMask)
STRUCTURE_SETTER(CommandBufferCount, const u32, commandBufferCount)
STRUCTURE_SETTER(PCommandBuffers, const VkCommandBuffer*, pCommandBuffers)
STRUCTURE_SETTER(SignalSemaphoreCount, const u32, signalSemaphoreCount)
STRUCTURE_SETTER(PSignalSemaphores, const VkSemaphore*, pSignalSemaphores)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_SUBMITINFO_H
