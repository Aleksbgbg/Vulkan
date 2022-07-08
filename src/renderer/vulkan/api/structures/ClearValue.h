#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_CLEARVALUE_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_CLEARVALUE_H_

#include <vulkan/vulkan.h>

#include "ClearColorValue.h"
#include "ClearDepthStencilValue.h"
#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER_NO_STYPE(ClearValueBuilder, VkClearValue)
STRUCTURE_SETTER(Color, VkClearColorValue, color)
STRUCTURE_SETTER(DepthStencil, VkClearDepthStencilValue, depthStencil)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_CLEARVALUE_H_
