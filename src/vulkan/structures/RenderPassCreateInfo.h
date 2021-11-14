#ifndef VULKAN_SRC_VULKAN_STRUCTURES_RENDERPASSCREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_RENDERPASSCREATEINFO_H

#include <vulkan/vulkan.h>

#include "AttachmentDescription.h"
#include "SubpassDependency.h"
#include "SubpassDescription.h"
#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(RenderPassCreateInfoBuilder, VkRenderPassCreateInfo,
                  VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO)
STRUCTURE_SETTER(Flags, VkRenderPassCreateFlags, flags)
STRUCTURE_SETTER(AttachmentCount, u32, attachmentCount)
STRUCTURE_SETTER(PAttachments, const VkAttachmentDescription*, pAttachments)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PAttachments,
                                      AttachmentDescriptionBuilder,
                                      pAttachments)
STRUCTURE_SETTER(SubpassCount, u32, subpassCount)
STRUCTURE_SETTER(PSubpasses, const VkSubpassDescription*, pSubpasses)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PSubpasses, SubpassDescriptionBuilder,
                                      pSubpasses)
STRUCTURE_SETTER(DependencyCount, u32, dependencyCount)
STRUCTURE_SETTER(PDependencies, const VkSubpassDependency*, pDependencies)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PDependencies, SubpassDependencyBuilder,
                                      pDependencies)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_RENDERPASSCREATEINFO_H
