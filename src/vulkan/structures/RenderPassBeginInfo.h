#ifndef VULKAN_SRC_VULKAN_STRUCTURES_RENDERPASSBEGININFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_RENDERPASSBEGININFO_H

#include <vulkan/vulkan_core.h>

#include "Rect2D.h"
#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER(RenderPassBeginInfoBuilder, VkRenderPassBeginInfo,
                  VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO)
STRUCTURE_SETTER(RenderPassBeginInfoBuilder, RenderPass, VkRenderPass,
                 renderPass)
STRUCTURE_SETTER(RenderPassBeginInfoBuilder, Framebuffer, VkFramebuffer,
                 framebuffer)
STRUCTURE_SETTER(RenderPassBeginInfoBuilder, RenderArea, VkRect2D, renderArea)
STRUCTURE_SETTER_BUILDER_OBJECT(RenderPassBeginInfoBuilder, RenderArea,
                                Rect2DBuilder, renderArea)
STRUCTURE_SETTER(RenderPassBeginInfoBuilder, ClearValueCount, u32,
                 clearValueCount)
STRUCTURE_SETTER(RenderPassBeginInfoBuilder, PClearValues, const VkClearValue*,
                 pClearValues)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_RENDERPASSBEGININFO_H
