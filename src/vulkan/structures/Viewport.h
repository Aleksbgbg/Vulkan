#ifndef VULKAN_SRC_VULKAN_STRUCTURES_VIEWPORT_H
#define VULKAN_SRC_VULKAN_STRUCTURES_VIEWPORT_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"

STRUCTURE_BUILDER_NO_TYPE(ViewportBuilder, VkViewport)
  STRUCTURE_SETTER(ViewportBuilder, X, const float, x)
  STRUCTURE_SETTER(ViewportBuilder, Y, const float, y)
  STRUCTURE_SETTER(ViewportBuilder, Width, const float, width)
  STRUCTURE_SETTER(ViewportBuilder, Height, const float, height)
  STRUCTURE_SETTER(ViewportBuilder, MinDepth, const float, minDepth)
  STRUCTURE_SETTER(ViewportBuilder, MaxDepth, const float, maxDepth)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_VIEWPORT_H
