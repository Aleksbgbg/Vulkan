#ifndef VULKAN_SRC_VULKAN_STRUCTURES_VERTEXINPUTBINDINGDESCRIPTION_H
#define VULKAN_SRC_VULKAN_STRUCTURES_VERTEXINPUTBINDINGDESCRIPTION_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER_NO_TYPE(VertexInputBindingDescriptionBuilder,
                          VkVertexInputBindingDescription)
STRUCTURE_SETTER(VertexInputBindingDescriptionBuilder, Binding, const u32,
                 binding)
STRUCTURE_SETTER(VertexInputBindingDescriptionBuilder, Stride, const u32,
                 stride)
STRUCTURE_SETTER(VertexInputBindingDescriptionBuilder, InputRate,
                 const VkVertexInputRate, inputRate)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_VERTEXINPUTBINDINGDESCRIPTION_H
