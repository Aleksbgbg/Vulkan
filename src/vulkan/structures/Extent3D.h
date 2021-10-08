#ifndef VULKAN_SRC_VULKAN_STRUCTURES_EXTENT3D_H
#define VULKAN_SRC_VULKAN_STRUCTURES_EXTENT3D_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER_NO_TYPE(Extent3DBuilder, VkExtent3D)
public:
explicit Extent3DBuilder(const VkExtent2D extent2d) : Extent3DBuilder() {
  value.width = extent2d.width;
  value.height = extent2d.height;
}
STRUCTURE_SETTER(Extent3DBuilder, Width, const u32, width)
STRUCTURE_SETTER(Extent3DBuilder, Height, const u32, height)
STRUCTURE_SETTER(Extent3DBuilder, Depth, const u32, depth)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_EXTENT3D_H
