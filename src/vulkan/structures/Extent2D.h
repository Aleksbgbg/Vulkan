#ifndef VULKAN_SRC_VULKAN_STRUCTURES_EXTENT2D_H_
#define VULKAN_SRC_VULKAN_STRUCTURES_EXTENT2D_H_

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER_NO_STYPE(Extent2DBuilder, VkExtent2D)
STRUCTURE_SETTER(Width, const u32, width)
STRUCTURE_SETTER(Height, const u32, height)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_EXTENT2D_H_
