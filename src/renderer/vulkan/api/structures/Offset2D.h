#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_OFFSET2D_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_OFFSET2D_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER_NO_STYPE(Offset2DBuilder, VkOffset2D)
STRUCTURE_SETTER(X, const i32, x)
STRUCTURE_SETTER(Y, const i32, y)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_OFFSET2D_H_
