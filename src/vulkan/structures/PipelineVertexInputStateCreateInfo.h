#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEVERTEXINPUTSTATECREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEVERTEXINPUTSTATECREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "VertexInputAttributeDescription.h"
#include "VertexInputBindingDescription.h"
#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER(PipelineVertexInputStateCreateInfoBuilder,
                  VkPipelineVertexInputStateCreateInfo,
                  VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO)
STRUCTURE_SETTER(VertexBindingDescriptionCount, const u32,
                 vertexBindingDescriptionCount)
STRUCTURE_SETTER(PVertexBindingDescriptions,
                 const VkVertexInputBindingDescription*,
                 pVertexBindingDescriptions)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PVertexBindingDescriptions,
                                      VertexInputBindingDescriptionBuilder,
                                      pVertexBindingDescriptions)
STRUCTURE_SETTER(VertexAttributeDescriptionCount, const u32,
                 vertexAttributeDescriptionCount)
STRUCTURE_SETTER(PVertexAttributeDescriptions,
                 const VkVertexInputAttributeDescription*,
                 pVertexAttributeDescriptions)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PVertexAttributeDescriptions,
                                      VertexInputAttributeDescriptionBuilder,
                                      pVertexAttributeDescriptions)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEVERTEXINPUTSTATECREATEINFO_H
