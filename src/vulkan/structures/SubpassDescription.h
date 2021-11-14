#ifndef VULKAN_SRC_VULKAN_STRUCTURES_SUBPASSDESCRIPTION_H
#define VULKAN_SRC_VULKAN_STRUCTURES_SUBPASSDESCRIPTION_H

#include <vulkan/vulkan.h>

#include "AttachmentReference.h"
#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER_NO_STYPE(SubpassDescriptionBuilder, VkSubpassDescription)
STRUCTURE_SETTER(Flags, VkSubpassDescriptionFlags, flags)
STRUCTURE_SETTER(PipelineBindPoint, VkPipelineBindPoint, pipelineBindPoint)
STRUCTURE_SETTER(InputAttachmentCount, u32, inputAttachmentCount)
STRUCTURE_SETTER(PInputAttachments, const VkAttachmentReference*,
                 pInputAttachments)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PInputAttachments,
                                      AttachmentReferenceBuilder,
                                      pInputAttachments)
STRUCTURE_SETTER(ColorAttachmentCount, u32, colorAttachmentCount)
STRUCTURE_SETTER(PColorAttachments, const VkAttachmentReference*,
                 pColorAttachments)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PColorAttachments,
                                      AttachmentReferenceBuilder,
                                      pColorAttachments)
STRUCTURE_SETTER(PResolveAttachments, const VkAttachmentReference*,
                 pResolveAttachments)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PResolveAttachments,
                                      AttachmentReferenceBuilder,
                                      pResolveAttachments)
STRUCTURE_SETTER(PDepthStencilAttachment, const VkAttachmentReference*,
                 pDepthStencilAttachment)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PDepthStencilAttachment,
                                      AttachmentReferenceBuilder,
                                      pDepthStencilAttachment)
STRUCTURE_SETTER(PreserveAttachmentCount, u32, preserveAttachmentCount)
STRUCTURE_SETTER(PPreserveAttachments, const u32*, pPreserveAttachments)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_SUBPASSDESCRIPTION_H
