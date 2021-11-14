#ifndef VULKAN_SRC_VULKAN_STRUCTURES_COMMANDPOOLCREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_COMMANDPOOLCREATEINFO_H

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(CommandPoolCreateInfoBuilder, VkCommandPoolCreateInfo,
                  VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO)
STRUCTURE_SETTER(Flags, const VkCommandPoolCreateFlags, flags)
STRUCTURE_SETTER(QueueFamilyIndex, const u32, queueFamilyIndex)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_COMMANDPOOLCREATEINFO_H
