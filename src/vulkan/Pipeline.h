#ifndef VULKAN_SRC_VULKAN_PIPELINE_H
#define VULKAN_SRC_VULKAN_PIPELINE_H

#include <vulkan/vulkan.h>

#include "lifetime_semantics.h"
#include "structures/GraphicsPipelineCreateInfo.h"
#include "ShaderModule.h"
#include "PipelineLayout.h"
#include "RenderPass.h"
#include "SubpassReference.h"

class Pipeline {
  friend class CommandBuffer;
public:
  Pipeline() = default;
  Pipeline(VkDevice shaderModule,
           const std::vector<ShaderModule>& shaderModules,
           PipelineLayout pipelineLayout,
           const SubpassReference subpassReference,
           GraphicsPipelineCreateInfoBuilder& infoBuilder);

  Pipeline(const Pipeline&) = delete;
  Pipeline(Pipeline&& other) noexcept
    :
      device(other.device),
      pipeline(other.pipeline),
      pipelineLayout(std::move(other.pipelineLayout)) {
    other.pipeline = nullptr;
  }

  ~Pipeline();

  Pipeline& operator=(const Pipeline&) = delete;
  Pipeline& operator=(Pipeline&& other) noexcept {
    device = other.device;
    std::swap(pipeline, other.pipeline);
    pipelineLayout = std::move(other.pipelineLayout);
    return *this;
  }

  PipelineLayout& GetLayout();

private:
  VkDevice device;
  VkPipeline pipeline = nullptr;
  PipelineLayout pipelineLayout;
};

#endif // VULKAN_SRC_VULKAN_PIPELINE_H
