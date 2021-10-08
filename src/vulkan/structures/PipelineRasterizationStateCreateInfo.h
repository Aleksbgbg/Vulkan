#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PIPELINERASTERIZATIONSTATECREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_PIPELINERASTERIZATIONSTATECREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER(PipelineRasterizationStateCreateInfoBuilder,
                  VkPipelineRasterizationStateCreateInfo,
                  VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO)
STRUCTURE_SETTER(PipelineRasterizationStateCreateInfoBuilder, DepthClampEnable,
                 const VkBool32, depthClampEnable)
STRUCTURE_SETTER(PipelineRasterizationStateCreateInfoBuilder,
                 RasterizerDiscardEnable, const VkBool32,
                 rasterizerDiscardEnable)
STRUCTURE_SETTER(PipelineRasterizationStateCreateInfoBuilder, PolygonMode,
                 const VkPolygonMode, polygonMode)
STRUCTURE_SETTER(PipelineRasterizationStateCreateInfoBuilder, CullMode,
                 const VkCullModeFlags, cullMode)
STRUCTURE_SETTER(PipelineRasterizationStateCreateInfoBuilder, FrontFace,
                 const VkFrontFace, frontFace)
STRUCTURE_SETTER(PipelineRasterizationStateCreateInfoBuilder, DepthBiasEnable,
                 const VkBool32, depthBiasEnable)
STRUCTURE_SETTER(PipelineRasterizationStateCreateInfoBuilder,
                 DepthBiasConstantFactor, const float, depthBiasConstantFactor)
STRUCTURE_SETTER(PipelineRasterizationStateCreateInfoBuilder, DepthBiasClamp,
                 const float, depthBiasClamp)
STRUCTURE_SETTER(PipelineRasterizationStateCreateInfoBuilder,
                 DepthBiasSlopeFactor, const float, depthBiasSlopeFactor)
STRUCTURE_SETTER(PipelineRasterizationStateCreateInfoBuilder, LineWidth,
                 const float, lineWidth)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_PIPELINERASTERIZATIONSTATECREATEINFO_H
