#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_RENDERPASSBEGININFO_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_RENDERPASSBEGININFO_H_

#include <vulkan/vulkan.h>

#include "Rect2D.h"
#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(RenderPassBeginInfoBuilder, VkRenderPassBeginInfo,
                  VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO)
STRUCTURE_SETTER(RenderPass, VkRenderPass, renderPass)
STRUCTURE_SETTER(Framebuffer, VkFramebuffer, framebuffer)
STRUCTURE_SETTER(RenderArea, VkRect2D, renderArea)
STRUCTURE_SETTER(ClearValueCount, u32, clearValueCount)
STRUCTURE_SETTER(PClearValues, const VkClearValue*, pClearValues)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_RENDERPASSBEGININFO_H_
