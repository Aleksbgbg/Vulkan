#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINEDEPTHSTENCILSTATECREATEINFO_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINEDEPTHSTENCILSTATECREATEINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

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

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINEDEPTHSTENCILSTATECREATEINFO_H_
