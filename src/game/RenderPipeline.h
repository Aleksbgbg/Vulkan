#ifndef VULKAN_SRC_GAME_RENDERPIPELINE_H
#define VULKAN_SRC_GAME_RENDERPIPELINE_H

#include <vulkan/vulkan.h>

#include "general/windowing/Window.h"
#include "vulkan/CommandBuffer.h"
#include "vulkan/Pipeline.h"

class ShaderModuleFactory {
 public:
  virtual ~ShaderModuleFactory() = default;

  virtual ShaderModule LoadShader(
      const VkShaderStageFlagBits stage,
      const std::string_view shaderFilename) const = 0;
};

class PipelineStateFactory {
 public:
  virtual ~PipelineStateFactory() = default;

  virtual std::vector<ShaderModule> LoadShaders(
      const ShaderModuleFactory& shaderModuleFactory) const = 0;
  virtual PipelineLayoutCreateInfoBuilder CreatePipelineLayout() const = 0;
  virtual PipelineVertexInputStateCreateInfoBuilder CreateVertexInputState()
      const = 0;
};

class RenderPipeline {
 public:
  class Initializer {
   public:
    virtual ~Initializer() = default;

    virtual Pipeline CreateGraphicsPipeline(
        const std::vector<ShaderModule>& shaders,
        const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts,
        const PipelineLayoutCreateInfoBuilder& pipelineLayout,
        const GraphicsPipelineCreateInfoBuilder& infoBuilder) const = 0;
    virtual PipelineMultisampleStateCreateInfoBuilder CreateMultisampleState()
        const = 0;
  };

  RenderPipeline() = default;
  RenderPipeline(
      const Initializer& initializer,
      const ShaderModuleFactory& shaderModuleFactory,
      const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts,
      const PipelineStateFactory& pipelineStateFactory);

  RenderPipeline(const RenderPipeline&) = delete;
  RenderPipeline(RenderPipeline&&) = default;

  RenderPipeline& operator=(const RenderPipeline&) = delete;
  RenderPipeline& operator=(RenderPipeline&&) = default;

  const PipelineLayout& GetLayout() const;
  void Bind(const CommandBuffer& commandBuffer,
            const wnd::Window& window) const;

 private:
  Pipeline pipeline;
};

#endif  // VULKAN_SRC_GAME_RENDERPIPELINE_H
