#ifndef VULKAN_SRC_VULKAN_STRUCTURES_SUBPASSDESCRIPTION_H
#define VULKAN_SRC_VULKAN_STRUCTURES_SUBPASSDESCRIPTION_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"
#include "AttachmentReference.h"

STRUCTURE_BUILDER_NO_TYPE(SubpassDescriptionBuilder, VkSubpassDescription)
  STRUCTURE_SETTER(SubpassDescriptionBuilder, Flags, VkSubpassDescriptionFlags, flags)
  STRUCTURE_SETTER(SubpassDescriptionBuilder, PipelineBindPoint, VkPipelineBindPoint, pipelineBindPoint)
  STRUCTURE_SETTER(SubpassDescriptionBuilder, InputAttachmentCount, u32, inputAttachmentCount)
  STRUCTURE_SETTER(SubpassDescriptionBuilder, PInputAttachments, const VkAttachmentReference*, pInputAttachments)
  STRUCTURE_SETTER_BUILDER(SubpassDescriptionBuilder, PInputAttachments, AttachmentReferenceBuilder, pInputAttachments)
  STRUCTURE_SETTER(SubpassDescriptionBuilder, ColorAttachmentCount, u32, colorAttachmentCount)
  STRUCTURE_SETTER(SubpassDescriptionBuilder, PColorAttachments, const VkAttachmentReference*, pColorAttachments)
  STRUCTURE_SETTER_BUILDER(SubpassDescriptionBuilder, PColorAttachments, AttachmentReferenceBuilder, pColorAttachments)
  STRUCTURE_SETTER(SubpassDescriptionBuilder, PResolveAttachments, const VkAttachmentReference*, pResolveAttachments)
  STRUCTURE_SETTER_BUILDER(SubpassDescriptionBuilder, PResolveAttachments, AttachmentReferenceBuilder, pResolveAttachments)
  STRUCTURE_SETTER(SubpassDescriptionBuilder, PDepthStencilAttachment, const VkAttachmentReference*, pDepthStencilAttachment)
  STRUCTURE_SETTER_BUILDER(SubpassDescriptionBuilder, PDepthStencilAttachment, AttachmentReferenceBuilder, pDepthStencilAttachment)
  STRUCTURE_SETTER(SubpassDescriptionBuilder, PreserveAttachmentCount, u32, preserveAttachmentCount)
  STRUCTURE_SETTER(SubpassDescriptionBuilder, PPreserveAttachments, const u32*, pPreserveAttachments)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_SUBPASSDESCRIPTION_H
