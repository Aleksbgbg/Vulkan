#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_FRAMEBUFFERCREATEINFO_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_FRAMEBUFFERCREATEINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

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

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_FRAMEBUFFERCREATEINFO_H_
