#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_SUBMITINFO_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_SUBMITINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

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

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_SUBMITINFO_H_
