#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_COMMANDPOOLCREATEINFO_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_COMMANDPOOLCREATEINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(CommandPoolCreateInfoBuilder, VkCommandPoolCreateInfo,
                  VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO)
STRUCTURE_SETTER(Flags, const VkCommandPoolCreateFlags, flags)
STRUCTURE_SETTER(QueueFamilyIndex, const u32, queueFamilyIndex)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_COMMANDPOOLCREATEINFO_H_
