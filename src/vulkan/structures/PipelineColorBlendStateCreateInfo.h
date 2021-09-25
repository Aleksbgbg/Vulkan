#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PIPELINECOLORBLENDSTATECREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_PIPELINECOLORBLENDSTATECREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(PipelineColorBlendStateCreateInfoBuilder, VkPipelineColorBlendStateCreateInfo, VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO)
  STRUCTURE_SETTER(PipelineColorBlendStateCreateInfoBuilder, Flags, VkPipelineColorBlendStateCreateFlags, flags)
  STRUCTURE_SETTER(PipelineColorBlendStateCreateInfoBuilder, LogicOpEnable, VkBool32, logicOpEnable)
  STRUCTURE_SETTER(PipelineColorBlendStateCreateInfoBuilder, LogicOp, VkLogicOp, logicOp)
  STRUCTURE_SETTER(PipelineColorBlendStateCreateInfoBuilder, AttachmentCount, u32, attachmentCount)
  STRUCTURE_SETTER(PipelineColorBlendStateCreateInfoBuilder, PAttachments, const VkPipelineColorBlendAttachmentState*, pAttachments)
  STRUCTURE_SETTER_BUILDER(PipelineColorBlendStateCreateInfoBuilder, PAttachments, PipelineColorBlendAttachmentStateBuilder, pAttachments)
  STRUCTURE_SETTER_DIFFERENT_ASSIGNMENT(PipelineColorBlendStateCreateInfoBuilder, BlendConstants0, const float, blendConstants, blendConstants[0])
  STRUCTURE_SETTER_DIFFERENT_ASSIGNMENT(PipelineColorBlendStateCreateInfoBuilder, BlendConstants1, const float, blendConstants, blendConstants[1])
  STRUCTURE_SETTER_DIFFERENT_ASSIGNMENT(PipelineColorBlendStateCreateInfoBuilder, BlendConstants2, const float, blendConstants, blendConstants[2])
  STRUCTURE_SETTER_DIFFERENT_ASSIGNMENT(PipelineColorBlendStateCreateInfoBuilder, BlendConstants3, const float, blendConstants, blendConstants[3])
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_PIPELINECOLORBLENDSTATECREATEINFO_H
