#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_COMMANDBUFFERINHERITANCEINFO_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_COMMANDBUFFERINHERITANCEINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(CommandBufferInheritanceInfoBuilder,
                  VkCommandBufferInheritanceInfo,
                  VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO)
STRUCTURE_SETTER(RenderPass, const VkRenderPass, renderPass)
STRUCTURE_SETTER(Subpass, const u32, subpass)
STRUCTURE_SETTER(Framebuffer, const VkFramebuffer, framebuffer)
STRUCTURE_SETTER(OcclusionQueryEnable, const VkBool32, occlusionQueryEnable)
STRUCTURE_SETTER(QueryFlags, const VkQueryControlFlags, queryFlags)
STRUCTURE_SETTER(PipelineStatistics, const VkQueryPipelineStatisticFlags,
                 pipelineStatistics)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_COMMANDBUFFERINHERITANCEINFO_H_
