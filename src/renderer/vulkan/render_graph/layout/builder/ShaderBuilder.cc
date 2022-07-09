#include "ShaderBuilder.h"

#include "asset.h"

ShaderBuilder::ShaderBuilder() : result_() {}

ShaderBuilder& ShaderBuilder::AddComputeShader(const asset::Shader shader) {
  return AddComputeShader(shader, DescriptorReferenceBuilder());
}

ShaderBuilder& ShaderBuilder::AddComputeShader(
    const asset::Shader shader, const DescriptorReferenceBuilder& builder) {
  result_.shader = CreateShader(shader, VK_SHADER_STAGE_COMPUTE_BIT, builder);
  return *this;
}

ShaderBuilder& ShaderBuilder::AddVertexShader(const asset::Shader shader) {
  return AddVertexShader(shader, DescriptorReferenceBuilder());
}

ShaderBuilder& ShaderBuilder::AddVertexShader(
    const asset::Shader shader, const DescriptorReferenceBuilder& builder) {
  result_.shaders.push_back(
      CreateShader(shader, VK_SHADER_STAGE_VERTEX_BIT, builder));
  return *this;
}

ShaderBuilder& ShaderBuilder::AddFragmentShader(const asset::Shader shader) {
  return AddFragmentShader(shader, DescriptorReferenceBuilder());
}

ShaderBuilder& ShaderBuilder::AddFragmentShader(
    const asset::Shader shader, const DescriptorReferenceBuilder& builder) {
  result_.shaders.push_back(
      CreateShader(shader, VK_SHADER_STAGE_FRAGMENT_BIT, builder));
  return *this;
}

ShaderBuilder::ShaderBuildResult ShaderBuilder::Build() const {
  return result_;
}

ShaderStructure ShaderBuilder::CreateShader(
    const asset::Shader shaderAsset, const VkShaderStageFlagBits stage,
    const DescriptorReferenceBuilder& descriptorReferenceBuilder) {
  DescriptorReferenceBuilder::DescriptorReferences references =
      descriptorReferenceBuilder.Build();
  return {.shaderAsset = shaderAsset,
          .shaderStage = stage,
          .globalBindings = std::move(references.globalBindings),
          .localBindings = std::move(references.localBindings)};
}
