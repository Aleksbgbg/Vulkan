#ifndef VULKAN_SRC_VULKAN_STRUCTURES_VERTEXINPUTBINDINGDESCRIPTION_H
#define VULKAN_SRC_VULKAN_STRUCTURES_VERTEXINPUTBINDINGDESCRIPTION_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER_NO_STYPE(VertexInputBindingDescriptionBuilder,
                           VkVertexInputBindingDescription)
STRUCTURE_SETTER(Binding, const u32, binding)
STRUCTURE_SETTER(Stride, const u32, stride)
STRUCTURE_SETTER(InputRate, const VkVertexInputRate, inputRate)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_VERTEXINPUTBINDINGDESCRIPTION_H
