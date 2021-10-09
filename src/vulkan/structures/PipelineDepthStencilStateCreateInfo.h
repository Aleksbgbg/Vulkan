#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEDEPTHSTENCILSTATECREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEDEPTHSTENCILSTATECREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER(PipelineDepthStencilStateCreateInfoBuilder,
                  VkPipelineDepthStencilStateCreateInfo,
                  VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO)
STRUCTURE_SETTER(Flags, VkPipelineDepthStencilStateCreateFlags, flags)
STRUCTURE_SETTER(DepthTestEnable, VkBool32, depthTestEnable)
STRUCTURE_SETTER(DepthWriteEnable, VkBool32, depthWriteEnable)
STRUCTURE_SETTER(DepthCompareOp, VkCompareOp, depthCompareOp)
STRUCTURE_SETTER(DepthBoundsTestEnable, VkBool32, depthBoundsTestEnable)
STRUCTURE_SETTER(StencilTestEnable, VkBool32, stencilTestEnable)
STRUCTURE_SETTER(Front, VkStencilOpState, front)
STRUCTURE_SETTER(Back, VkStencilOpState, back)
STRUCTURE_SETTER(MinDepthBounds, float, minDepthBounds)
STRUCTURE_SETTER(MaxDepthBounds, float, maxDepthBounds)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEDEPTHSTENCILSTATECREATEINFO_H
