#ifndef VULKAN_SRC_VULKAN_STRUCTURES_RENDERPASSCREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_RENDERPASSCREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "AttachmentDescription.h"
#include "SubpassDependency.h"
#include "SubpassDescription.h"
#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER(RenderPassCreateInfoBuilder, VkRenderPassCreateInfo,
                  VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO)
STRUCTURE_SETTER(RenderPassCreateInfoBuilder, Flags, VkRenderPassCreateFlags,
                 flags)
STRUCTURE_SETTER(RenderPassCreateInfoBuilder, AttachmentCount, u32,
                 attachmentCount)
STRUCTURE_SETTER(RenderPassCreateInfoBuilder, PAttachments,
                 const VkAttachmentDescription*, pAttachments)
STRUCTURE_SETTER_BUILDER(RenderPassCreateInfoBuilder, PAttachments,
                         AttachmentDescriptionBuilder, pAttachments)
STRUCTURE_SETTER(RenderPassCreateInfoBuilder, SubpassCount, u32, subpassCount)
STRUCTURE_SETTER(RenderPassCreateInfoBuilder, PSubpasses,
                 const VkSubpassDescription*, pSubpasses)
STRUCTURE_SETTER_BUILDER(RenderPassCreateInfoBuilder, PSubpasses,
                         SubpassDescriptionBuilder, pSubpasses)
STRUCTURE_SETTER(RenderPassCreateInfoBuilder, DependencyCount, u32,
                 dependencyCount)
STRUCTURE_SETTER(RenderPassCreateInfoBuilder, PDependencies,
                 const VkSubpassDependency*, pDependencies)
STRUCTURE_SETTER_BUILDER(RenderPassCreateInfoBuilder, PDependencies,
                         SubpassDependencyBuilder, pDependencies)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_RENDERPASSCREATEINFO_H
