#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_VIEWPORT_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_VIEWPORT_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER_NO_STYPE(ViewportBuilder, VkViewport)
STRUCTURE_SETTER(X, const float, x)
STRUCTURE_SETTER(Y, const float, y)
STRUCTURE_SETTER(Width, const float, width)
STRUCTURE_SETTER(Height, const float, height)
STRUCTURE_SETTER(MinDepth, const float, minDepth)
STRUCTURE_SETTER(MaxDepth, const float, maxDepth)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_VIEWPORT_H_
