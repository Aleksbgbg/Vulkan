#ifndef SRC_RENDERER_VULKAN_API_GRAPHICSPIPELINE_H_
#define SRC_RENDERER_VULKAN_API_GRAPHICSPIPELINE_H_

#include <vulkan/vulkan.h>

#include "PipelineCache.h"
#include "PipelineLayout.h"
#include "RenderPass.h"
#include "ShaderModule.h"
#include "SubpassReference.h"
#include "renderer/vulkan/api/structures/GraphicsPipelineCreateInfo.h"

namespace vk {

class GraphicsPipeline {
  friend class CommandBuffer;

 public:
  GraphicsPipeline();
  GraphicsPipeline(VkDevice device, const PipelineCache& pipelineCache,
                   const std::vector<ShaderModule>& shaderModules,
                   PipelineLayout pipelineLayout,
                   const SubpassReference subpassReference,
                   GraphicsPipelineCreateInfoBuilder infoBuilder);

  GraphicsPipeline(const GraphicsPipeline&) = delete;
  GraphicsPipeline(GraphicsPipeline&& other) noexcept;

  ~GraphicsPipeline();

  GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;
  GraphicsPipeline& operator=(GraphicsPipeline&& other) noexcept;

  const PipelineLayout& GetLayout() const;

 private:
  VkDevice device;
  VkPipeline pipeline;
  PipelineLayout pipelineLayout;
};

}  // namespace vk

#endif  // SRC_RENDERER_VULKAN_API_GRAPHICSPIPELINE_H_
