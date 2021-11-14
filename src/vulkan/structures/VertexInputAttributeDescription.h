#ifndef VULKAN_SRC_VULKAN_STRUCTURES_VERTEXINPUTATTRIBUTEDESCRIPTION_H
#define VULKAN_SRC_VULKAN_STRUCTURES_VERTEXINPUTATTRIBUTEDESCRIPTION_H

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER_NO_STYPE(VertexInputAttributeDescriptionBuilder,
                           VkVertexInputAttributeDescription)
STRUCTURE_SETTER(Location, const u32, location)
STRUCTURE_SETTER(Binding, const u32, binding)
STRUCTURE_SETTER(Format, const VkFormat, format)
STRUCTURE_SETTER(Offset, const u32, offset)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_VERTEXINPUTATTRIBUTEDESCRIPTION_H
