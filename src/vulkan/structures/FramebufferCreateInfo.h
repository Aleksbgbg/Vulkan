#ifndef VULKAN_SRC_VULKAN_STRUCTURES_FRAMEBUFFERCREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_FRAMEBUFFERCREATEINFO_H

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(FramebufferCreateInfoBuilder, VkFramebufferCreateInfo,
                  VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO)
STRUCTURE_SETTER(Flags, VkFramebufferCreateFlags, flags)
STRUCTURE_SETTER(RenderPass, VkRenderPass, renderPass)
STRUCTURE_SETTER(AttachmentCount, u32, attachmentCount)
STRUCTURE_SETTER(PAttachments, const VkImageView*, pAttachments)
STRUCTURE_SETTER(Width, u32, width)
STRUCTURE_SETTER(Height, u32, height)
STRUCTURE_SETTER(Layers, u32, layers)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_FRAMEBUFFERCREATEINFO_H
