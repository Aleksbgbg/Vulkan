#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_SPECIALIZATIONMAPENTRY_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_SPECIALIZATIONMAPENTRY_H_

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER_NO_STYPE(SpecializationMapEntryBuilder,
                           VkSpecializationMapEntry)
STRUCTURE_SETTER(ConstantID, const u32, constantID)
STRUCTURE_SETTER(Offset, const u32, offset)
STRUCTURE_SETTER(Size, const size_t, size)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_SPECIALIZATIONMAPENTRY_H_