#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINETESSELLATIONSTATECREATEINFO_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINETESSELLATIONSTATECREATEINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(PipelineTessellationStateCreateInfoBuilder,
                  VkPipelineTessellationStateCreateInfo,
                  VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO)
STRUCTURE_SETTER(Flags, VkPipelineTessellationStateCreateFlags, flags)
STRUCTURE_SETTER(PatchControlPoints, u32, patchControlPoints)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINETESSELLATIONSTATECREATEINFO_H_
