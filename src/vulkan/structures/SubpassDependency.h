#ifndef VULKAN_SRC_VULKAN_STRUCTURES_SUBPASSDEPENDENCY_H_
#define VULKAN_SRC_VULKAN_STRUCTURES_SUBPASSDEPENDENCY_H_

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER_NO_STYPE(SubpassDependencyBuilder, VkSubpassDependency)
STRUCTURE_SETTER(SrcSubpass, u32, srcSubpass)
STRUCTURE_SETTER(DstSubpass, u32, dstSubpass)
STRUCTURE_SETTER(SrcStageMask, VkPipelineStageFlags, srcStageMask)
STRUCTURE_SETTER(DstStageMask, VkPipelineStageFlags, dstStageMask)
STRUCTURE_SETTER(SrcAccessMask, VkAccessFlags, srcAccessMask)
STRUCTURE_SETTER(DstAccessMask, VkAccessFlags, dstAccessMask)
STRUCTURE_SETTER(DependencyFlags, VkDependencyFlags, dependencyFlags)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_SUBPASSDEPENDENCY_H_
