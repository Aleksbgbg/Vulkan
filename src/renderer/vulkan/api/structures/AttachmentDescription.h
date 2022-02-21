#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_ATTACHMENTDESCRIPTION_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_ATTACHMENTDESCRIPTION_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

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

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_ATTACHMENTDESCRIPTION_H_
