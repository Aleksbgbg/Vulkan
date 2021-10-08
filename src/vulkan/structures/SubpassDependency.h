#ifndef VULKAN_SRC_VULKAN_STRUCTURES_SUBPASSDEPENDENCY_H
#define VULKAN_SRC_VULKAN_STRUCTURES_SUBPASSDEPENDENCY_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER_NO_TYPE(SubpassDependencyBuilder, VkSubpassDependency)
STRUCTURE_SETTER(SubpassDependencyBuilder, SrcSubpass, u32, srcSubpass)
STRUCTURE_SETTER(SubpassDependencyBuilder, DstSubpass, u32, dstSubpass)
STRUCTURE_SETTER(SubpassDependencyBuilder, SrcStageMask, VkPipelineStageFlags,
                 srcStageMask)
STRUCTURE_SETTER(SubpassDependencyBuilder, DstStageMask, VkPipelineStageFlags,
                 dstStageMask)
STRUCTURE_SETTER(SubpassDependencyBuilder, SrcAccessMask, VkAccessFlags,
                 srcAccessMask)
STRUCTURE_SETTER(SubpassDependencyBuilder, DstAccessMask, VkAccessFlags,
                 dstAccessMask)
STRUCTURE_SETTER(SubpassDependencyBuilder, DependencyFlags, VkDependencyFlags,
                 dependencyFlags)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_SUBPASSDEPENDENCY_H
