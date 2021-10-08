#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PIPELINECOLORBLENDATTACHMENTSTATE_H
#define VULKAN_SRC_VULKAN_STRUCTURES_PIPELINECOLORBLENDATTACHMENTSTATE_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER_NO_TYPE(PipelineColorBlendAttachmentStateBuilder,
                          VkPipelineColorBlendAttachmentState)
STRUCTURE_SETTER(PipelineColorBlendAttachmentStateBuilder, BlendEnable,
                 VkBool32, blendEnable)
STRUCTURE_SETTER(PipelineColorBlendAttachmentStateBuilder, SrcColorBlendFactor,
                 VkBlendFactor, srcColorBlendFactor)
STRUCTURE_SETTER(PipelineColorBlendAttachmentStateBuilder, DstColorBlendFactor,
                 VkBlendFactor, dstColorBlendFactor)
STRUCTURE_SETTER(PipelineColorBlendAttachmentStateBuilder, ColorBlendOp,
                 VkBlendOp, colorBlendOp)
STRUCTURE_SETTER(PipelineColorBlendAttachmentStateBuilder, SrcAlphaBlendFactor,
                 VkBlendFactor, srcAlphaBlendFactor)
STRUCTURE_SETTER(PipelineColorBlendAttachmentStateBuilder, DstAlphaBlendFactor,
                 VkBlendFactor, dstAlphaBlendFactor)
STRUCTURE_SETTER(PipelineColorBlendAttachmentStateBuilder, AlphaBlendOp,
                 VkBlendOp, alphaBlendOp)
STRUCTURE_SETTER(PipelineColorBlendAttachmentStateBuilder, ColorWriteMask,
                 VkColorComponentFlags, colorWriteMask)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_PIPELINECOLORBLENDATTACHMENTSTATE_H
