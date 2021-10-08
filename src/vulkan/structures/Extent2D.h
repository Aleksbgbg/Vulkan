#ifndef VULKAN_SRC_VULKAN_STRUCTURES_EXTENT2D_H
#define VULKAN_SRC_VULKAN_STRUCTURES_EXTENT2D_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER_NO_TYPE(Extent2DBuilder, VkExtent2D)
STRUCTURE_SETTER(Extent2DBuilder, Width, const u32, width)
STRUCTURE_SETTER(Extent2DBuilder, Height, const u32, height)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_EXTENT2D_H
