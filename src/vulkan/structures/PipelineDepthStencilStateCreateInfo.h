#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEDEPTHSTENCILSTATECREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEDEPTHSTENCILSTATECREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER(PipelineDepthStencilStateCreateInfoBuilder,
                  VkPipelineDepthStencilStateCreateInfo,
                  VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO)
STRUCTURE_SETTER(PipelineDepthStencilStateCreateInfoBuilder, Flags,
                 VkPipelineDepthStencilStateCreateFlags, flags)
STRUCTURE_SETTER(PipelineDepthStencilStateCreateInfoBuilder, DepthTestEnable,
                 VkBool32, depthTestEnable)
STRUCTURE_SETTER(PipelineDepthStencilStateCreateInfoBuilder, DepthWriteEnable,
                 VkBool32, depthWriteEnable)
STRUCTURE_SETTER(PipelineDepthStencilStateCreateInfoBuilder, DepthCompareOp,
                 VkCompareOp, depthCompareOp)
STRUCTURE_SETTER(PipelineDepthStencilStateCreateInfoBuilder,
                 DepthBoundsTestEnable, VkBool32, depthBoundsTestEnable)
STRUCTURE_SETTER(PipelineDepthStencilStateCreateInfoBuilder, StencilTestEnable,
                 VkBool32, stencilTestEnable)
STRUCTURE_SETTER(PipelineDepthStencilStateCreateInfoBuilder, Front,
                 VkStencilOpState, front)
STRUCTURE_SETTER(PipelineDepthStencilStateCreateInfoBuilder, Back,
                 VkStencilOpState, back)
STRUCTURE_SETTER(PipelineDepthStencilStateCreateInfoBuilder, MinDepthBounds,
                 float, minDepthBounds)
STRUCTURE_SETTER(PipelineDepthStencilStateCreateInfoBuilder, MaxDepthBounds,
                 float, maxDepthBounds)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEDEPTHSTENCILSTATECREATEINFO_H
