#ifndef VULKAN_SRC_VULKAN_STRUCTURES_COMMANDBUFFERALLOCATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_COMMANDBUFFERALLOCATEINFO_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(CommandBufferAllocateInfoBuilder, VkCommandBufferAllocateInfo, VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO)
  STRUCTURE_SETTER(CommandBufferAllocateInfoBuilder, CommandPool, VkCommandPool, commandPool)
  STRUCTURE_SETTER(CommandBufferAllocateInfoBuilder, Level, VkCommandBufferLevel, level)
  STRUCTURE_SETTER(CommandBufferAllocateInfoBuilder, CommandBufferCount, const u32, commandBufferCount)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_COMMANDBUFFERALLOCATEINFO_H
