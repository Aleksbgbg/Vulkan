#ifndef VULKAN_SRC_VULKAN_STRUCTURES_GRAPHICSPIPELINECREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_GRAPHICSPIPELINECREATEINFO_H

#include <vulkan/vulkan_core.h>

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
#include "types.h"

STRUCTURE_BUILDER(GraphicsPipelineCreateInfoBuilder,
                  VkGraphicsPipelineCreateInfo,
                  VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO)
STRUCTURE_SETTER(GraphicsPipelineCreateInfoBuilder, Flags,
                 VkPipelineCreateFlags, flags)
STRUCTURE_SETTER(GraphicsPipelineCreateInfoBuilder, StageCount, u32, stageCount)
STRUCTURE_SETTER(GraphicsPipelineCreateInfoBuilder, PStages,
                 const VkPipelineShaderStageCreateInfo*, pStages)
STRUCTURE_SETTER(GraphicsPipelineCreateInfoBuilder, PVertexInputState,
                 const VkPipelineVertexInputStateCreateInfo*, pVertexInputState)
STRUCTURE_SETTER_BUILDER(GraphicsPipelineCreateInfoBuilder, PVertexInputState,
                         PipelineVertexInputStateCreateInfoBuilder,
                         pVertexInputState)
STRUCTURE_SETTER(GraphicsPipelineCreateInfoBuilder, PInputAssemblyState,
                 const VkPipelineInputAssemblyStateCreateInfo*,
                 pInputAssemblyState)
STRUCTURE_SETTER_BUILDER(GraphicsPipelineCreateInfoBuilder, PInputAssemblyState,
                         PipelineInputAssemblyStateCreateInfoBuilder,
                         pInputAssemblyState)
STRUCTURE_SETTER(GraphicsPipelineCreateInfoBuilder, PTessellationState,
                 const VkPipelineTessellationStateCreateInfo*,
                 pTessellationState)
STRUCTURE_SETTER_BUILDER(GraphicsPipelineCreateInfoBuilder, PTessellationState,
                         PipelineTessellationStateCreateInfoBuilder,
                         pTessellationState)
STRUCTURE_SETTER(GraphicsPipelineCreateInfoBuilder, PViewportState,
                 const VkPipelineViewportStateCreateInfo*, pViewportState)
STRUCTURE_SETTER_BUILDER(GraphicsPipelineCreateInfoBuilder, PViewportState,
                         PipelineViewportStateCreateInfoBuilder, pViewportState)
STRUCTURE_SETTER(GraphicsPipelineCreateInfoBuilder, PRasterizationState,
                 const VkPipelineRasterizationStateCreateInfo*,
                 pRasterizationState)
STRUCTURE_SETTER_BUILDER(GraphicsPipelineCreateInfoBuilder, PRasterizationState,
                         PipelineRasterizationStateCreateInfoBuilder,
                         pRasterizationState)
STRUCTURE_SETTER(GraphicsPipelineCreateInfoBuilder, PMultisampleState,
                 const VkPipelineMultisampleStateCreateInfo*, pMultisampleState)
STRUCTURE_SETTER_BUILDER(GraphicsPipelineCreateInfoBuilder, PMultisampleState,
                         PipelineMultisampleStateCreateInfoBuilder,
                         pMultisampleState)
STRUCTURE_SETTER(GraphicsPipelineCreateInfoBuilder, PDepthStencilState,
                 const VkPipelineDepthStencilStateCreateInfo*,
                 pDepthStencilState)
STRUCTURE_SETTER_BUILDER(GraphicsPipelineCreateInfoBuilder, PDepthStencilState,
                         PipelineDepthStencilStateCreateInfoBuilder,
                         pDepthStencilState)
STRUCTURE_SETTER(GraphicsPipelineCreateInfoBuilder, PColorBlendState,
                 const VkPipelineColorBlendStateCreateInfo*, pColorBlendState)
STRUCTURE_SETTER_BUILDER(GraphicsPipelineCreateInfoBuilder, PColorBlendState,
                         PipelineColorBlendStateCreateInfoBuilder,
                         pColorBlendState)
STRUCTURE_SETTER(GraphicsPipelineCreateInfoBuilder, PDynamicState,
                 const VkPipelineDynamicStateCreateInfo*, pDynamicState)
STRUCTURE_SETTER_BUILDER(GraphicsPipelineCreateInfoBuilder, PDynamicState,
                         PipelineDynamicStateCreateInfoBuilder, pDynamicState)
STRUCTURE_SETTER(GraphicsPipelineCreateInfoBuilder, Layout, VkPipelineLayout,
                 layout)
STRUCTURE_SETTER(GraphicsPipelineCreateInfoBuilder, RenderPass, VkRenderPass,
                 renderPass)
STRUCTURE_SETTER(GraphicsPipelineCreateInfoBuilder, Subpass, u32, subpass)
STRUCTURE_SETTER(GraphicsPipelineCreateInfoBuilder, BasePipelineHandle,
                 VkPipeline, basePipelineHandle)
STRUCTURE_SETTER(GraphicsPipelineCreateInfoBuilder, BasePipelineIndex, i32,
                 basePipelineIndex)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_GRAPHICSPIPELINECREATEINFO_H
