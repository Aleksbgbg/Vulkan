#ifndef SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_SHADERBUILDER_H_
#define SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_SHADERBUILDER_H_

#include "DescriptorReferenceBuilder.h"
#include "renderer/vulkan/render_graph/layout/ShaderStructure.h"

class ShaderBuilder {
 public:
  ShaderBuilder();

  ShaderBuilder& AddComputeShader();
  ShaderBuilder& AddComputeShader(const DescriptorReferenceBuilder& builder);
  ShaderBuilder& AddVertexShader();
  ShaderBuilder& AddVertexShader(const DescriptorReferenceBuilder& builder);
  ShaderBuilder& AddFragmentShader();
  ShaderBuilder& AddFragmentShader(const DescriptorReferenceBuilder& builder);

  struct ShaderBuildResult {
    ShaderStructure shader;
    std::list<ShaderStructure> shaders;
  };

  ShaderBuildResult Build() const;

 private:
  static ShaderStructure CreateShader(
      VkShaderStageFlagBits stage,
      const DescriptorReferenceBuilder& descriptorReferenceBuilder);

 private:
  ShaderBuildResult result_;
};

#endif  // SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_SHADERBUILDER_H_
