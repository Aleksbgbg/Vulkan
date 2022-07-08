#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_RECT2D_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_RECT2D_H_

#include <vulkan/vulkan.h>

#include "Extent2D.h"
#include "Offset2D.h"
#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER_NO_STYPE(Rect2DBuilder, VkRect2D)
STRUCTURE_SETTER(Offset, const VkOffset2D, offset)
STRUCTURE_SETTER(Extent, const VkExtent2D, extent)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_RECT2D_H_
