#ifndef VULKAN_SRC_GAME_RENDERPIPELINE_H
#define VULKAN_SRC_GAME_RENDERPIPELINE_H

#include <vulkan/vulkan.h>

#include "general/windowing/Window.h"
#include "vulkan/CommandBuffer.h"
#include "vulkan/Pipeline.h"

class PipelineStateFactory {
 public:
  virtual ~PipelineStateFactory() = default;

  virtual std::vector<ShaderModule> LoadShaders(
      const VirtualDevice& virtualDevice) const = 0;
  virtual PipelineLayoutCreateInfoBuilder CreatePipelineLayout() const = 0;
  virtual PipelineVertexInputStateCreateInfoBuilder CreateVertexInputState()
      const = 0;
};

class RenderPipeline {
 public:
  RenderPipeline() = default;
  RenderPipeline(
      const VirtualDevice& virtualDevice, const PipelineCache& pipelineCache,
      const std::vector<const DescriptorSetLayout*> descriptorSetLayouts,
      const SubpassReference subpassReference,
      const VkSampleCountFlagBits samples,
      const PipelineStateFactory& pipelineStateFactory);

  RenderPipeline(const RenderPipeline&) = delete;
  RenderPipeline(RenderPipeline&&) = default;

  RenderPipeline& operator=(const RenderPipeline&) = delete;
  RenderPipeline& operator=(RenderPipeline&&) = default;

  const PipelineLayout& GetLayout() const;
  void Bind(const CommandBuffer& commandBuffer, const Window& window) const;

 private:
  Pipeline pipeline;
};

#endif  // VULKAN_SRC_GAME_RENDERPIPELINE_H
