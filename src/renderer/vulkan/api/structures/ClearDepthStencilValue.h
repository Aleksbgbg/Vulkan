#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_CLEARDEPTHSTENCILVALUE_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_CLEARDEPTHSTENCILVALUE_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER_NO_STYPE(ClearDepthStencilValueBuilder,
                           VkClearDepthStencilValue)
STRUCTURE_SETTER(Depth, float, depth)
STRUCTURE_SETTER(Stencil, u32, stencil)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_CLEARDEPTHSTENCILVALUE_H_
