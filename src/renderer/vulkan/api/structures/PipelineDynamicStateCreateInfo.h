#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINEDYNAMICSTATECREATEINFO_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINEDYNAMICSTATECREATEINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(PipelineDynamicStateCreateInfoBuilder,
                  VkPipelineDynamicStateCreateInfo,
                  VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO)
STRUCTURE_SETTER(Flags, VkPipelineDynamicStateCreateFlags, flags)
STRUCTURE_SETTER(DynamicStateCount, u32, dynamicStateCount)
STRUCTURE_SETTER(PDynamicStates, const VkDynamicState*, pDynamicStates)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINEDYNAMICSTATECREATEINFO_H_
