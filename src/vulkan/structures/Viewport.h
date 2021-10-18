#ifndef VULKAN_SRC_VULKAN_STRUCTURES_VIEWPORT_H
#define VULKAN_SRC_VULKAN_STRUCTURES_VIEWPORT_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER_NO_STYPE(ViewportBuilder, VkViewport)
STRUCTURE_SETTER(X, const float, x)
STRUCTURE_SETTER(Y, const float, y)
STRUCTURE_SETTER(Width, const float, width)
STRUCTURE_SETTER(Height, const float, height)
STRUCTURE_SETTER(MinDepth, const float, minDepth)
STRUCTURE_SETTER(MaxDepth, const float, maxDepth)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_VIEWPORT_H
