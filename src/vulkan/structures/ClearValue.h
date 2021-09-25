#ifndef VULKAN_SRC_VULKAN_STRUCTURES_CLEARVALUE_H
#define VULKAN_SRC_VULKAN_STRUCTURES_CLEARVALUE_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"
#include "ClearColorValue.h"
#include "ClearDepthStencilValue.h"

STRUCTURE_BUILDER_NO_TYPE(ClearValueBuilder, VkClearValue)
  STRUCTURE_SETTER(ClearValueBuilder, Color, VkClearColorValue, color)
  STRUCTURE_SETTER_BUILDER_OBJECT(ClearValueBuilder, Color, ClearColorValueBuilder, color)
  STRUCTURE_SETTER(ClearValueBuilder, DepthStencil, VkClearDepthStencilValue, depthStencil)
  STRUCTURE_SETTER_BUILDER_OBJECT(ClearValueBuilder, DepthStencil, ClearDepthStencilValueBuilder, depthStencil)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_CLEARVALUE_H
