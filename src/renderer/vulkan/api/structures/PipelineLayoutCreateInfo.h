#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINELAYOUTCREATEINFO_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINELAYOUTCREATEINFO_H_

#include <vulkan/vulkan.h>

#include "PushConstantRange.h"
#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(PipelineLayoutCreateInfoBuilder, VkPipelineLayoutCreateInfo,
                  VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO)
STRUCTURE_SETTER(SetLayoutCount, const u32, setLayoutCount)
STRUCTURE_SETTER(PSetLayouts, const VkDescriptorSetLayout*, pSetLayouts)
STRUCTURE_SETTER(PushConstantRangeCount, const u32, pushConstantRangeCount)
STRUCTURE_SETTER(PPushConstantRanges, const VkPushConstantRange*,
                 pPushConstantRanges)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PPushConstantRanges,
                                      PushConstantRangeBuilder,
                                      pPushConstantRanges)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINELAYOUTCREATEINFO_H_
