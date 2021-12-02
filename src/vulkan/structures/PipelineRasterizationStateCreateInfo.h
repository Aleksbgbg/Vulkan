#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PIPELINERASTERIZATIONSTATECREATEINFO_H_
#define VULKAN_SRC_VULKAN_STRUCTURES_PIPELINERASTERIZATIONSTATECREATEINFO_H_

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(PipelineRasterizationStateCreateInfoBuilder,
                  VkPipelineRasterizationStateCreateInfo,
                  VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO)
STRUCTURE_SETTER(DepthClampEnable, const VkBool32, depthClampEnable)
STRUCTURE_SETTER(RasterizerDiscardEnable, const VkBool32,
                 rasterizerDiscardEnable)
STRUCTURE_SETTER(PolygonMode, const VkPolygonMode, polygonMode)
STRUCTURE_SETTER(CullMode, const VkCullModeFlags, cullMode)
STRUCTURE_SETTER(FrontFace, const VkFrontFace, frontFace)
STRUCTURE_SETTER(DepthBiasEnable, const VkBool32, depthBiasEnable)
STRUCTURE_SETTER(DepthBiasConstantFactor, const float, depthBiasConstantFactor)
STRUCTURE_SETTER(DepthBiasClamp, const float, depthBiasClamp)
STRUCTURE_SETTER(DepthBiasSlopeFactor, const float, depthBiasSlopeFactor)
STRUCTURE_SETTER(LineWidth, const float, lineWidth)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_PIPELINERASTERIZATIONSTATECREATEINFO_H_
