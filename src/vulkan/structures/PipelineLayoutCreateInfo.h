#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PIPELINELAYOUTCREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_PIPELINELAYOUTCREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"
#include "PushConstantRange.h"

STRUCTURE_BUILDER(PipelineLayoutCreateInfoBuilder, VkPipelineLayoutCreateInfo, VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO)
  STRUCTURE_SETTER(PipelineLayoutCreateInfoBuilder, SetLayoutCount, const u32, setLayoutCount)
  STRUCTURE_SETTER(PipelineLayoutCreateInfoBuilder, PSetLayouts, const VkDescriptorSetLayout*, pSetLayouts)
  STRUCTURE_SETTER(PipelineLayoutCreateInfoBuilder, PushConstantRangeCount, const u32, pushConstantRangeCount)
  STRUCTURE_SETTER(PipelineLayoutCreateInfoBuilder, PPushConstantRanges, const VkPushConstantRange*, pPushConstantRanges)
  STRUCTURE_SETTER_BUILDER(PipelineLayoutCreateInfoBuilder, PPushConstantRanges, PushConstantRangeBuilder, pPushConstantRanges)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_PIPELINELAYOUTCREATEINFO_H
