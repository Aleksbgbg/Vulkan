#ifndef VULKAN_SRC_VULKAN_STRUCTURES_OFFSET2D_H
#define VULKAN_SRC_VULKAN_STRUCTURES_OFFSET2D_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"

STRUCTURE_BUILDER_NO_TYPE(Offset2DBuilder, VkOffset2D)
  STRUCTURE_SETTER(Offset2DBuilder, X, const i32, x)
  STRUCTURE_SETTER(Offset2DBuilder, Y, const i32, y)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_OFFSET2D_H
