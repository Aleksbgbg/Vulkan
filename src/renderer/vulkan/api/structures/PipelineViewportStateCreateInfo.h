#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINEVIEWPORTSTATECREATEINFO_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINEVIEWPORTSTATECREATEINFO_H_

#include <vulkan/vulkan.h>

#include "Rect2D.h"
#include "Viewport.h"
#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(PipelineViewportStateCreateInfoBuilder,
                  VkPipelineViewportStateCreateInfo,
                  VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO)
STRUCTURE_SETTER(ViewportCount, const u32, viewportCount)
STRUCTURE_SETTER(PViewports, const VkViewport*, pViewports)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PViewports, ViewportBuilder, pViewports)
STRUCTURE_SETTER(ScissorCount, const u32, scissorCount)
STRUCTURE_SETTER(PScissors, const VkRect2D*, pScissors)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PScissors, Rect2DBuilder, pScissors)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINEVIEWPORTSTATECREATEINFO_H_
