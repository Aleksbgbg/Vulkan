#ifndef VULKAN_SRC_VULKAN_STRUCTURES_COMMANDBUFFERBEGININFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_COMMANDBUFFERBEGININFO_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(CommandBufferBeginInfoBuilder, VkCommandBufferBeginInfo, VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO)
  STRUCTURE_SETTER(CommandBufferBeginInfoBuilder, Flags, const VkCommandBufferUsageFlags, flags)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_COMMANDBUFFERBEGININFO_H
