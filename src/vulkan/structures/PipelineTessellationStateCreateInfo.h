#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PIPELINETESSELLATIONSTATECREATEINFO_H_
#define VULKAN_SRC_VULKAN_STRUCTURES_PIPELINETESSELLATIONSTATECREATEINFO_H_

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(PipelineTessellationStateCreateInfoBuilder,
                  VkPipelineTessellationStateCreateInfo,
                  VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO)
STRUCTURE_SETTER(Flags, VkPipelineTessellationStateCreateFlags, flags)
STRUCTURE_SETTER(PatchControlPoints, u32, patchControlPoints)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_PIPELINETESSELLATIONSTATECREATEINFO_H_
