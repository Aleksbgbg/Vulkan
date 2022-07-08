#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_FENCECREATEINFO_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_FENCECREATEINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(FenceCreateInfoBuilder, VkFenceCreateInfo,
                  VK_STRUCTURE_TYPE_FENCE_CREATE_INFO)
STRUCTURE_SETTER(Flags, const VkFenceCreateFlags, flags)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_FENCECREATEINFO_H_
