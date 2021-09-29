#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PUSHCONSTANTRANGE_H
#define VULKAN_SRC_VULKAN_STRUCTURES_PUSHCONSTANTRANGE_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"

STRUCTURE_BUILDER_NO_TYPE(PushConstantRangeBuilder, VkPushConstantRange)
  STRUCTURE_SETTER(PushConstantRangeBuilder, StageFlags, VkShaderStageFlags, stageFlags)
  STRUCTURE_SETTER(PushConstantRangeBuilder, Offset, u32, offset)
  STRUCTURE_SETTER(PushConstantRangeBuilder, Size, u32, size)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_PUSHCONSTANTRANGE_H
