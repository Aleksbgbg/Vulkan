#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PUSHCONSTANTRANGE_H
#define VULKAN_SRC_VULKAN_STRUCTURES_PUSHCONSTANTRANGE_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER_NO_STYPE(PushConstantRangeBuilder, VkPushConstantRange)
STRUCTURE_SETTER(StageFlags, VkShaderStageFlags, stageFlags)
STRUCTURE_SETTER(Offset, u32, offset)
STRUCTURE_SETTER(Size, u32, size)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_PUSHCONSTANTRANGE_H
