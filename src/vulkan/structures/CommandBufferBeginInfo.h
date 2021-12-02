#ifndef VULKAN_SRC_VULKAN_STRUCTURES_COMMANDBUFFERBEGININFO_H_
#define VULKAN_SRC_VULKAN_STRUCTURES_COMMANDBUFFERBEGININFO_H_

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(CommandBufferBeginInfoBuilder, VkCommandBufferBeginInfo,
                  VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO)
STRUCTURE_SETTER(Flags, const VkCommandBufferUsageFlags, flags)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_COMMANDBUFFERBEGININFO_H_
