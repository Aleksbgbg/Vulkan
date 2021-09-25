#ifndef VULKAN_SRC_VULKAN_STRUCTURES_COMMANDPOOLCREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_COMMANDPOOLCREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(CommandPoolCreateInfoBuilder, VkCommandPoolCreateInfo, VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO)
  STRUCTURE_SETTER(CommandPoolCreateInfoBuilder, Flags, const VkCommandPoolCreateFlags, flags)
  STRUCTURE_SETTER(CommandPoolCreateInfoBuilder, QueueFamilyIndex, const u32, queueFamilyIndex)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_COMMANDPOOLCREATEINFO_H
