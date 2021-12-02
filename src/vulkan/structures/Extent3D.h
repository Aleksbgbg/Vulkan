#ifndef VULKAN_SRC_VULKAN_STRUCTURES_EXTENT3D_H_
#define VULKAN_SRC_VULKAN_STRUCTURES_EXTENT3D_H_

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER_NO_STYPE(Extent3DBuilder, VkExtent3D)
public:
explicit Extent3DBuilder(const VkExtent2D extent2d) : Extent3DBuilder() {
  value.width = extent2d.width;
  value.height = extent2d.height;
}
STRUCTURE_SETTER(Width, const u32, width)
STRUCTURE_SETTER(Height, const u32, height)
STRUCTURE_SETTER(Depth, const u32, depth)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_EXTENT3D_H_
