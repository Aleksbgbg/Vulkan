#ifndef VULKAN_SRC_VULKAN_PIPELINE_H
#define VULKAN_SRC_VULKAN_PIPELINE_H

#include <vulkan/vulkan.h>

#include "PipelineCache.h"
#include "PipelineLayout.h"
#include "RenderPass.h"
#include "ShaderModule.h"
#include "SubpassReference.h"
#include "structures/GraphicsPipelineCreateInfo.h"

class Pipeline {
  friend class CommandBuffer;

 public:
  Pipeline();
  Pipeline(VkDevice shaderModule, const PipelineCache& pipelineCache,
           const std::vector<ShaderModule>& shaderModules,
           PipelineLayout pipelineLayout,
           const SubpassReference subpassReference,
           GraphicsPipelineCreateInfoBuilder infoBuilder);

  Pipeline(const Pipeline&) = delete;
  Pipeline(Pipeline&& other) noexcept;

  ~Pipeline();

  Pipeline& operator=(const Pipeline&) = delete;
  Pipeline& operator=(Pipeline&& other) noexcept;

  const PipelineLayout& GetLayout() const;

 private:
  VkDevice device;
  VkPipeline pipeline;
  PipelineLayout pipelineLayout;
};

#endif  // VULKAN_SRC_VULKAN_PIPELINE_H
