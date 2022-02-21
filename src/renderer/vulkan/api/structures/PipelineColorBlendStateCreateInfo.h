#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINECOLORBLENDSTATECREATEINFO_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINECOLORBLENDSTATECREATEINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(PipelineColorBlendStateCreateInfoBuilder,
                  VkPipelineColorBlendStateCreateInfo,
                  VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO)
STRUCTURE_SETTER(Flags, VkPipelineColorBlendStateCreateFlags, flags)
STRUCTURE_SETTER(LogicOpEnable, VkBool32, logicOpEnable)
STRUCTURE_SETTER(LogicOp, VkLogicOp, logicOp)
STRUCTURE_SETTER(AttachmentCount, u32, attachmentCount)
STRUCTURE_SETTER(PAttachments, const VkPipelineColorBlendAttachmentState*,
                 pAttachments)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PAttachments,
                                      PipelineColorBlendAttachmentStateBuilder,
                                      pAttachments)
STRUCTURE_SETTER_CUSTOM_ASSIGNMENT(BlendConstants0, const float, blendConstants,
                                   blendConstants[0])
STRUCTURE_SETTER_CUSTOM_ASSIGNMENT(BlendConstants1, const float, blendConstants,
                                   blendConstants[1])
STRUCTURE_SETTER_CUSTOM_ASSIGNMENT(BlendConstants2, const float, blendConstants,
                                   blendConstants[2])
STRUCTURE_SETTER_CUSTOM_ASSIGNMENT(BlendConstants3, const float, blendConstants,
                                   blendConstants[3])
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINECOLORBLENDSTATECREATEINFO_H_
