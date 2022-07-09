#ifndef SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_SHADERBUILDER_H_
#define SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_SHADERBUILDER_H_

#include "DescriptorReferenceBuilder.h"
#include "asset.h"
#include "renderer/vulkan/render_graph/layout/ShaderStructure.h"

class ShaderBuilder {
 public:
  ShaderBuilder();

  ShaderBuilder& AddComputeShader(asset::Shader shader);
  ShaderBuilder& AddComputeShader(asset::Shader shader,
                                  const DescriptorReferenceBuilder& builder);
  ShaderBuilder& AddVertexShader(asset::Shader shader);
  ShaderBuilder& AddVertexShader(asset::Shader shader,
                                 const DescriptorReferenceBuilder& builder);
  ShaderBuilder& AddFragmentShader(asset::Shader shader);
  ShaderBuilder& AddFragmentShader(asset::Shader shader,
                                   const DescriptorReferenceBuilder& builder);

  struct ShaderBuildResult {
    ShaderStructure shader;
    std::list<ShaderStructure> shaders;
  };

  ShaderBuildResult Build() const;

 private:
  static ShaderStructure CreateShader(
      asset::Shader shaderAsset, const VkShaderStageFlagBits stage,
      const DescriptorReferenceBuilder& descriptorReferenceBuilder);

 private:
  ShaderBuildResult result_;
};

#endif  // SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_SHADERBUILDER_H_
