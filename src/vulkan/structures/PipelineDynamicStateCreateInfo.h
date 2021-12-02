#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEDYNAMICSTATECREATEINFO_H_
#define VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEDYNAMICSTATECREATEINFO_H_

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(PipelineDynamicStateCreateInfoBuilder,
                  VkPipelineDynamicStateCreateInfo,
                  VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO)
STRUCTURE_SETTER(Flags, VkPipelineDynamicStateCreateFlags, flags)
STRUCTURE_SETTER(DynamicStateCount, u32, dynamicStateCount)
STRUCTURE_SETTER(PDynamicStates, const VkDynamicState*, pDynamicStates)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEDYNAMICSTATECREATEINFO_H_
