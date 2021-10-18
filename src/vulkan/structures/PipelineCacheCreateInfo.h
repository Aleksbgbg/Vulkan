#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PIPELINECACHECREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_PIPELINECACHECREATEINFO_H

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(PipelineCacheCreateInfoBuilder, VkPipelineCacheCreateInfo,
                  VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO)
STRUCTURE_SETTER(Flags, VkPipelineCacheCreateFlags, flags)
STRUCTURE_SETTER(InitialDataSize, size_t, initialDataSize)
STRUCTURE_SETTER(PInitialData, const void*, pInitialData)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_PIPELINECACHECREATEINFO_H
