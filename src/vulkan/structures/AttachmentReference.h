#ifndef VULKAN_SRC_VULKAN_STRUCTURES_ATTACHMENTREFERENCE_H
#define VULKAN_SRC_VULKAN_STRUCTURES_ATTACHMENTREFERENCE_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER_NO_TYPE(AttachmentReferenceBuilder, VkAttachmentReference)
STRUCTURE_SETTER(AttachmentReferenceBuilder, Attachment, u32, attachment)
STRUCTURE_SETTER(AttachmentReferenceBuilder, Layout, VkImageLayout, layout)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_ATTACHMENTREFERENCE_H
