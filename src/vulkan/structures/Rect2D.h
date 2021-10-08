#ifndef VULKAN_SRC_VULKAN_STRUCTURES_RECT2D_H
#define VULKAN_SRC_VULKAN_STRUCTURES_RECT2D_H

#include <vulkan/vulkan_core.h>

#include "Extent2D.h"
#include "Offset2D.h"
#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER_NO_TYPE(Rect2DBuilder, VkRect2D)
STRUCTURE_SETTER(Rect2DBuilder, Offset, const VkOffset2D, offset)
STRUCTURE_SETTER_BUILDER_OBJECT(Rect2DBuilder, Offset, Offset2DBuilder, offset)
STRUCTURE_SETTER(Rect2DBuilder, Extent, const VkExtent2D, extent)
STRUCTURE_SETTER_BUILDER_OBJECT(Rect2DBuilder, Extent, Extent2DBuilder, extent)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_RECT2D_H
