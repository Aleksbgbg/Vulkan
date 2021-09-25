#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEVIEWPORTSTATECREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEVIEWPORTSTATECREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"
#include "Viewport.h"
#include "Rect2D.h"

STRUCTURE_BUILDER(PipelineViewportStateCreateInfoBuilder, VkPipelineViewportStateCreateInfo, VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO)
  STRUCTURE_SETTER(PipelineViewportStateCreateInfoBuilder, ViewportCount, const u32, viewportCount)
  STRUCTURE_SETTER(PipelineViewportStateCreateInfoBuilder, PViewports, const VkViewport*, pViewports)
  STRUCTURE_SETTER_BUILDER(PipelineViewportStateCreateInfoBuilder, PViewports, ViewportBuilder, pViewports)
  STRUCTURE_SETTER(PipelineViewportStateCreateInfoBuilder, ScissorCount, const u32, scissorCount)
  STRUCTURE_SETTER(PipelineViewportStateCreateInfoBuilder, PScissors, const VkRect2D*, pScissors)
  STRUCTURE_SETTER_BUILDER(PipelineViewportStateCreateInfoBuilder, PScissors, Rect2DBuilder, pScissors)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEVIEWPORTSTATECREATEINFO_H
