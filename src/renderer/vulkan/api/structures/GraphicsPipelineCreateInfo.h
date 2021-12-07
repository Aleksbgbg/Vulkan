#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_GRAPHICSPIPELINECREATEINFO_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_GRAPHICSPIPELINECREATEINFO_H_

#include <vulkan/vulkan.h>

#include "PipelineColorBlendAttachmentState.h"
#include "PipelineColorBlendStateCreateInfo.h"
#include "PipelineDepthStencilStateCreateInfo.h"
#include "PipelineDynamicStateCreateInfo.h"
#include "PipelineInputAssemblyStateCreateInfo.h"
#include "PipelineLayoutCreateInfo.h"
#include "PipelineMultisampleStateCreateInfo.h"
#include "PipelineRasterizationStateCreateInfo.h"
#include "PipelineShaderStageCreateInfo.h"
#include "PipelineTessellationStateCreateInfo.h"
#include "PipelineVertexInputStateCreateInfo.h"
#include "PipelineViewportStateCreateInfo.h"
#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(GraphicsPipelineCreateInfoBuilder,
                  VkGraphicsPipelineCreateInfo,
                  VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO)
STRUCTURE_SETTER(Flags, VkPipelineCreateFlags, flags)
STRUCTURE_SETTER(StageCount, u32, stageCount)
STRUCTURE_SETTER(PStages, const VkPipelineShaderStageCreateInfo*, pStages)
STRUCTURE_SETTER(PVertexInputState, const VkPipelineVertexInputStateCreateInfo*,
                 pVertexInputState)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PVertexInputState,
                                      PipelineVertexInputStateCreateInfoBuilder,
                                      pVertexInputState)
STRUCTURE_SETTER(PInputAssemblyState,
                 const VkPipelineInputAssemblyStateCreateInfo*,
                 pInputAssemblyState)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(
    PInputAssemblyState, PipelineInputAssemblyStateCreateInfoBuilder,
    pInputAssemblyState)
STRUCTURE_SETTER(PTessellationState,
                 const VkPipelineTessellationStateCreateInfo*,
                 pTessellationState)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(
    PTessellationState, PipelineTessellationStateCreateInfoBuilder,
    pTessellationState)
STRUCTURE_SETTER(PViewportState, const VkPipelineViewportStateCreateInfo*,
                 pViewportState)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PViewportState,
                                      PipelineViewportStateCreateInfoBuilder,
                                      pViewportState)
STRUCTURE_SETTER(PRasterizationState,
                 const VkPipelineRasterizationStateCreateInfo*,
                 pRasterizationState)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(
    PRasterizationState, PipelineRasterizationStateCreateInfoBuilder,
    pRasterizationState)
STRUCTURE_SETTER(PMultisampleState, const VkPipelineMultisampleStateCreateInfo*,
                 pMultisampleState)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PMultisampleState,
                                      PipelineMultisampleStateCreateInfoBuilder,
                                      pMultisampleState)
STRUCTURE_SETTER(PDepthStencilState,
                 const VkPipelineDepthStencilStateCreateInfo*,
                 pDepthStencilState)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(
    PDepthStencilState, PipelineDepthStencilStateCreateInfoBuilder,
    pDepthStencilState)
STRUCTURE_SETTER(PColorBlendState, const VkPipelineColorBlendStateCreateInfo*,
                 pColorBlendState)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PColorBlendState,
                                      PipelineColorBlendStateCreateInfoBuilder,
                                      pColorBlendState)
STRUCTURE_SETTER(PDynamicState, const VkPipelineDynamicStateCreateInfo*,
                 pDynamicState)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PDynamicState,
                                      PipelineDynamicStateCreateInfoBuilder,
                                      pDynamicState)
STRUCTURE_SETTER(Layout, VkPipelineLayout, layout)
STRUCTURE_SETTER(RenderPass, VkRenderPass, renderPass)
STRUCTURE_SETTER(Subpass, u32, subpass)
STRUCTURE_SETTER(BasePipelineHandle, VkPipeline, basePipelineHandle)
STRUCTURE_SETTER(BasePipelineIndex, i32, basePipelineIndex)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_GRAPHICSPIPELINECREATEINFO_H_
