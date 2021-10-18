#ifndef VULKAN_SRC_VULKAN_STRUCTURES_ATTACHMENTDESCRIPTION_H
#define VULKAN_SRC_VULKAN_STRUCTURES_ATTACHMENTDESCRIPTION_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER_NO_STYPE(AttachmentDescriptionBuilder,
                           VkAttachmentDescription)
STRUCTURE_SETTER(Flags, VkAttachmentDescriptionFlags, flags)
STRUCTURE_SETTER(Format, VkFormat, format)
STRUCTURE_SETTER(Samples, VkSampleCountFlagBits, samples)
STRUCTURE_SETTER(LoadOp, VkAttachmentLoadOp, loadOp)
STRUCTURE_SETTER(StoreOp, VkAttachmentStoreOp, storeOp)
STRUCTURE_SETTER(StencilLoadOp, VkAttachmentLoadOp, stencilLoadOp)
STRUCTURE_SETTER(StencilStoreOp, VkAttachmentStoreOp, stencilStoreOp)
STRUCTURE_SETTER(InitialLayout, VkImageLayout, initialLayout)
STRUCTURE_SETTER(FinalLayout, VkImageLayout, finalLayout)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_ATTACHMENTDESCRIPTION_H
