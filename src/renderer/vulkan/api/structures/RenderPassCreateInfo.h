#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_RENDERPASSCREATEINFO_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_RENDERPASSCREATEINFO_H_

#include <vulkan/vulkan.h>

#include "AttachmentDescription.h"
#include "SubpassDependency.h"
#include "SubpassDescription.h"
#include "core/types.h"
#include "define_structure.h"

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

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_RENDERPASSCREATEINFO_H_
