#ifndef VULKAN_SRC_VULKAN_STRUCTURES_OFFSET2D_H
#define VULKAN_SRC_VULKAN_STRUCTURES_OFFSET2D_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER_NO_STYPE(Offset2DBuilder, VkOffset2D)
STRUCTURE_SETTER(X, const i32, x)
STRUCTURE_SETTER(Y, const i32, y)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_OFFSET2D_H
