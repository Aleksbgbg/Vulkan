#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEDYNAMICSTATECREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEDYNAMICSTATECREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER(PipelineDynamicStateCreateInfoBuilder,
                  VkPipelineDynamicStateCreateInfo,
                  VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO)
STRUCTURE_SETTER(Flags, VkPipelineDynamicStateCreateFlags, flags)
STRUCTURE_SETTER(DynamicStateCount, u32, dynamicStateCount)
STRUCTURE_SETTER(PDynamicStates, const VkDynamicState*, pDynamicStates)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEDYNAMICSTATECREATEINFO_H
