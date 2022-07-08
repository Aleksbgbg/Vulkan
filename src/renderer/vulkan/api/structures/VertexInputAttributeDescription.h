#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_VERTEXINPUTATTRIBUTEDESCRIPTION_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_VERTEXINPUTATTRIBUTEDESCRIPTION_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER_NO_STYPE(VertexInputAttributeDescriptionBuilder,
                           VkVertexInputAttributeDescription)
STRUCTURE_SETTER(Location, const u32, location)
STRUCTURE_SETTER(Binding, const u32, binding)
STRUCTURE_SETTER(Format, const VkFormat, format)
STRUCTURE_SETTER(Offset, const u32, offset)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_VERTEXINPUTATTRIBUTEDESCRIPTION_H_
