#ifndef VULKAN_SRC_VULKAN_STRUCTURES_CLEARVALUE_H
#define VULKAN_SRC_VULKAN_STRUCTURES_CLEARVALUE_H

#include <vulkan/vulkan_core.h>

#include "ClearColorValue.h"
#include "ClearDepthStencilValue.h"
#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER_NO_STYPE(ClearValueBuilder, VkClearValue)
STRUCTURE_SETTER(Color, VkClearColorValue, color)
STRUCTURE_SETTER(DepthStencil, VkClearDepthStencilValue, depthStencil)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_CLEARVALUE_H
