#ifndef VULKAN_SRC_VULKAN_STRUCTURES_FENCECREATEINFO_H_
#define VULKAN_SRC_VULKAN_STRUCTURES_FENCECREATEINFO_H_

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(FenceCreateInfoBuilder, VkFenceCreateInfo,
                  VK_STRUCTURE_TYPE_FENCE_CREATE_INFO)
STRUCTURE_SETTER(Flags, const VkFenceCreateFlags, flags)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_FENCECREATEINFO_H_
