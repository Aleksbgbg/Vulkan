#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_VERTEXINPUTBINDINGDESCRIPTION_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_VERTEXINPUTBINDINGDESCRIPTION_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER_NO_STYPE(VertexInputBindingDescriptionBuilder,
                           VkVertexInputBindingDescription)
STRUCTURE_SETTER(Binding, const u32, binding)
STRUCTURE_SETTER(Stride, const u32, stride)
STRUCTURE_SETTER(InputRate, const VkVertexInputRate, inputRate)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_VERTEXINPUTBINDINGDESCRIPTION_H_
