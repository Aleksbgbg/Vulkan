#ifndef VULKAN_SRC_VULKAN_STRUCTURES_CLEARDEPTHSTENCILVALUE_H
#define VULKAN_SRC_VULKAN_STRUCTURES_CLEARDEPTHSTENCILVALUE_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER_NO_TYPE(ClearDepthStencilValueBuilder,
                          VkClearDepthStencilValue)
STRUCTURE_SETTER(ClearDepthStencilValueBuilder, Depth, float, depth)
STRUCTURE_SETTER(ClearDepthStencilValueBuilder, Stencil, u32, stencil)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_CLEARDEPTHSTENCILVALUE_H
