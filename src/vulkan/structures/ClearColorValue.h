#ifndef VULKAN_SRC_VULKAN_STRUCTURES_CLEARCOLORVALUE_H
#define VULKAN_SRC_VULKAN_STRUCTURES_CLEARCOLORVALUE_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"

STRUCTURE_BUILDER_NO_TYPE(ClearColorValueBuilder, VkClearColorValue)
  STRUCTURE_SETTER_DIFFERENT_ASSIGNMENT(ClearColorValueBuilder, Float0, float, float32, float32[0])
  STRUCTURE_SETTER_DIFFERENT_ASSIGNMENT(ClearColorValueBuilder, Float1, float, float32, float32[1])
  STRUCTURE_SETTER_DIFFERENT_ASSIGNMENT(ClearColorValueBuilder, Float2, float, float32, float32[2])
  STRUCTURE_SETTER_DIFFERENT_ASSIGNMENT(ClearColorValueBuilder, Float3, float, float32, float32[3])
  STRUCTURE_SETTER_DIFFERENT_ASSIGNMENT(ClearColorValueBuilder, Int0, i32, int32, int32[0])
  STRUCTURE_SETTER_DIFFERENT_ASSIGNMENT(ClearColorValueBuilder, Int1, i32, int32, int32[1])
  STRUCTURE_SETTER_DIFFERENT_ASSIGNMENT(ClearColorValueBuilder, Int2, i32, int32, int32[2])
  STRUCTURE_SETTER_DIFFERENT_ASSIGNMENT(ClearColorValueBuilder, Int3, i32, int32, int32[3])
  STRUCTURE_SETTER_DIFFERENT_ASSIGNMENT(ClearColorValueBuilder, UInt0, u32, uint32, uint32[0])
  STRUCTURE_SETTER_DIFFERENT_ASSIGNMENT(ClearColorValueBuilder, UInt1, u32, uint32, uint32[1])
  STRUCTURE_SETTER_DIFFERENT_ASSIGNMENT(ClearColorValueBuilder, UInt2, u32, uint32, uint32[2])
  STRUCTURE_SETTER_DIFFERENT_ASSIGNMENT(ClearColorValueBuilder, UInt3, u32, uint32, uint32[3])
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_CLEARCOLORVALUE_H
