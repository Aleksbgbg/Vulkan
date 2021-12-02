#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PUSHCONSTANTRANGE_H_
#define VULKAN_SRC_VULKAN_STRUCTURES_PUSHCONSTANTRANGE_H_

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER_NO_STYPE(PushConstantRangeBuilder, VkPushConstantRange)
STRUCTURE_SETTER(StageFlags, VkShaderStageFlags, stageFlags)
STRUCTURE_SETTER(Offset, u32, offset)
STRUCTURE_SETTER(Size, u32, size)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_PUSHCONSTANTRANGE_H_
