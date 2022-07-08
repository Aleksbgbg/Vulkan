#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_COMMANDBUFFERALLOCATEINFO_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_COMMANDBUFFERALLOCATEINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(CommandBufferAllocateInfoBuilder, VkCommandBufferAllocateInfo,
                  VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO)
STRUCTURE_SETTER(CommandPool, VkCommandPool, commandPool)
STRUCTURE_SETTER(Level, VkCommandBufferLevel, level)
STRUCTURE_SETTER(CommandBufferCount, const u32, commandBufferCount)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_COMMANDBUFFERALLOCATEINFO_H_
