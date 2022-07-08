#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_EXTENT2D_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_EXTENT2D_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER_NO_STYPE(Extent2DBuilder, VkExtent2D)
STRUCTURE_SETTER(Width, const u32, width)
STRUCTURE_SETTER(Height, const u32, height)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_EXTENT2D_H_
