#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_SPECIALIZATIONINFO_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_SPECIALIZATIONINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER_NO_STYPE(SpecializationInfoBuilder, VkSpecializationInfo)
STRUCTURE_SETTER(MapEntryCount, const u32, mapEntryCount)
STRUCTURE_SETTER(PMapEntries, const VkSpecializationMapEntry* const,
                 pMapEntries)
STRUCTURE_SETTER(DataSize, const size_t, dataSize)
STRUCTURE_SETTER(PData, const void* const, pData)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_SPECIALIZATIONINFO_H_
