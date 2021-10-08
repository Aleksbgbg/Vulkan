#ifndef VULKAN_SRC_VULKAN_STRUCTURES_ATTACHMENTDESCRIPTION_H
#define VULKAN_SRC_VULKAN_STRUCTURES_ATTACHMENTDESCRIPTION_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER_NO_TYPE(AttachmentDescriptionBuilder, VkAttachmentDescription)
STRUCTURE_SETTER(AttachmentDescriptionBuilder, Flags,
                 VkAttachmentDescriptionFlags, flags)
STRUCTURE_SETTER(AttachmentDescriptionBuilder, Format, VkFormat, format)
STRUCTURE_SETTER(AttachmentDescriptionBuilder, Samples, VkSampleCountFlagBits,
                 samples)
STRUCTURE_SETTER(AttachmentDescriptionBuilder, LoadOp, VkAttachmentLoadOp,
                 loadOp)
STRUCTURE_SETTER(AttachmentDescriptionBuilder, StoreOp, VkAttachmentStoreOp,
                 storeOp)
STRUCTURE_SETTER(AttachmentDescriptionBuilder, StencilLoadOp,
                 VkAttachmentLoadOp, stencilLoadOp)
STRUCTURE_SETTER(AttachmentDescriptionBuilder, StencilStoreOp,
                 VkAttachmentStoreOp, stencilStoreOp)
STRUCTURE_SETTER(AttachmentDescriptionBuilder, InitialLayout, VkImageLayout,
                 initialLayout)
STRUCTURE_SETTER(AttachmentDescriptionBuilder, FinalLayout, VkImageLayout,
                 finalLayout)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_ATTACHMENTDESCRIPTION_H
