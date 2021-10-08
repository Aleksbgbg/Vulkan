#ifndef VULKAN_SRC_VULKAN_STRUCTURES_VERTEXINPUTATTRIBUTEDESCRIPTION_H
#define VULKAN_SRC_VULKAN_STRUCTURES_VERTEXINPUTATTRIBUTEDESCRIPTION_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER_NO_TYPE(VertexInputAttributeDescriptionBuilder,
                          VkVertexInputAttributeDescription)
STRUCTURE_SETTER(VertexInputAttributeDescriptionBuilder, Location, const u32,
                 location)
STRUCTURE_SETTER(VertexInputAttributeDescriptionBuilder, Binding, const u32,
                 binding)
STRUCTURE_SETTER(VertexInputAttributeDescriptionBuilder, Format, const VkFormat,
                 format)
STRUCTURE_SETTER(VertexInputAttributeDescriptionBuilder, Offset, const u32,
                 offset)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_VERTEXINPUTATTRIBUTEDESCRIPTION_H
