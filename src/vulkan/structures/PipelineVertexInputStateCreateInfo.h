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
STRUCTURE_SETTER(PipelineVertexInputStateCreateInfoBuilder,
                 VertexBindingDescriptionCount, const u32,
                 vertexBindingDescriptionCount)
STRUCTURE_SETTER(PipelineVertexInputStateCreateInfoBuilder,
                 PVertexBindingDescriptions,
                 const VkVertexInputBindingDescription*,
                 pVertexBindingDescriptions)
STRUCTURE_SETTER_BUILDER(PipelineVertexInputStateCreateInfoBuilder,
                         PVertexBindingDescriptions,
                         VertexInputBindingDescriptionBuilder,
                         pVertexBindingDescriptions)
STRUCTURE_SETTER(PipelineVertexInputStateCreateInfoBuilder,
                 VertexAttributeDescriptionCount, const u32,
                 vertexAttributeDescriptionCount)
STRUCTURE_SETTER(PipelineVertexInputStateCreateInfoBuilder,
                 PVertexAttributeDescriptions,
                 const VkVertexInputAttributeDescription*,
                 pVertexAttributeDescriptions)
STRUCTURE_SETTER_BUILDER(PipelineVertexInputStateCreateInfoBuilder,
                         PVertexAttributeDescriptions,
                         VertexInputAttributeDescriptionBuilder,
                         pVertexAttributeDescriptions)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEVERTEXINPUTSTATECREATEINFO_H
