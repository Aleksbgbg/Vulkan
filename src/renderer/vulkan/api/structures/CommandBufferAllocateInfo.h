#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_COMMANDBUFFERALLOCATEINFO_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_COMMANDBUFFERALLOCATEINFO_H_

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(CommandBufferAllocateInfoBuilder, VkCommandBufferAllocateInfo,
                  VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO)
STRUCTURE_SETTER(CommandPool, VkCommandPool, commandPool)
STRUCTURE_SETTER(Level, VkCommandBufferLevel, level)
STRUCTURE_SETTER(CommandBufferCount, const u32, commandBufferCount)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_COMMANDBUFFERALLOCATEINFO_H_
