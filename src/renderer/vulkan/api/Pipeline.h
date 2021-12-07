#ifndef VULKAN_SRC_RENDERER_VULKAN_API_PIPELINE_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_PIPELINE_H_

#include <vulkan/vulkan.h>

#include "PipelineCache.h"
#include "PipelineLayout.h"
#include "RenderPass.h"
#include "ShaderModule.h"
#include "SubpassReference.h"
#include "renderer/vulkan/api/structures/GraphicsPipelineCreateInfo.h"

class Pipeline {
  friend class CommandBuffer;

 public:
  Pipeline();
  Pipeline(VkDevice device, const PipelineCache& pipelineCache,
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

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_PIPELINE_H_
