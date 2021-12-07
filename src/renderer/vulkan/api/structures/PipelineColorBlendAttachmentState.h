#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINECOLORBLENDATTACHMENTSTATE_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINECOLORBLENDATTACHMENTSTATE_H_

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER_NO_STYPE(PipelineColorBlendAttachmentStateBuilder,
                           VkPipelineColorBlendAttachmentState)
STRUCTURE_SETTER(BlendEnable, VkBool32, blendEnable)
STRUCTURE_SETTER(SrcColorBlendFactor, VkBlendFactor, srcColorBlendFactor)
STRUCTURE_SETTER(DstColorBlendFactor, VkBlendFactor, dstColorBlendFactor)
STRUCTURE_SETTER(ColorBlendOp, VkBlendOp, colorBlendOp)
STRUCTURE_SETTER(SrcAlphaBlendFactor, VkBlendFactor, srcAlphaBlendFactor)
STRUCTURE_SETTER(DstAlphaBlendFactor, VkBlendFactor, dstAlphaBlendFactor)
STRUCTURE_SETTER(AlphaBlendOp, VkBlendOp, alphaBlendOp)
STRUCTURE_SETTER(ColorWriteMask, VkColorComponentFlags, colorWriteMask)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINECOLORBLENDATTACHMENTSTATE_H_
