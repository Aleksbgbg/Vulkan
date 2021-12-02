#ifndef VULKAN_SRC_VULKAN_STRUCTURES_ATTACHMENTREFERENCE_H_
#define VULKAN_SRC_VULKAN_STRUCTURES_ATTACHMENTREFERENCE_H_

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER_NO_STYPE(AttachmentReferenceBuilder, VkAttachmentReference)
STRUCTURE_SETTER(Attachment, u32, attachment)
STRUCTURE_SETTER(Layout, VkImageLayout, layout)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_ATTACHMENTREFERENCE_H_
