#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_COMMANDBUFFERBEGININFO_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_COMMANDBUFFERBEGININFO_H_

#include <vulkan/vulkan.h>

#include "CommandBufferInheritanceInfo.h"
#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(CommandBufferBeginInfoBuilder, VkCommandBufferBeginInfo,
                  VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO)
STRUCTURE_SETTER(Flags, const VkCommandBufferUsageFlags, flags)
STRUCTURE_SETTER(PInheritanceInfo, const VkCommandBufferInheritanceInfo* const,
                 pInheritanceInfo)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PInheritanceInfo,
                                      CommandBufferInheritanceInfoBuilder,
                                      pInheritanceInfo)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_COMMANDBUFFERBEGININFO_H_
