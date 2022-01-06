#include "ShaderBuilder.h"

ShaderBuilder::ShaderBuilder() : result_() {}

ShaderBuilder& ShaderBuilder::AddComputeShader() {
  return AddComputeShader(DescriptorReferenceBuilder());
}

ShaderBuilder& ShaderBuilder::AddComputeShader(
    const DescriptorReferenceBuilder& builder) {
  result_.shader = CreateShader(VK_SHADER_STAGE_COMPUTE_BIT, builder);
  return *this;
}

ShaderBuilder& ShaderBuilder::AddVertexShader() {
  return AddVertexShader(DescriptorReferenceBuilder());
}

ShaderBuilder& ShaderBuilder::AddVertexShader(
    const DescriptorReferenceBuilder& builder) {
  result_.shaders.push_back(CreateShader(VK_SHADER_STAGE_VERTEX_BIT, builder));
  return *this;
}

ShaderBuilder& ShaderBuilder::AddFragmentShader() {
  return AddFragmentShader(DescriptorReferenceBuilder());
}

ShaderBuilder& ShaderBuilder::AddFragmentShader(
    const DescriptorReferenceBuilder& builder) {
  result_.shaders.push_back(
      CreateShader(VK_SHADER_STAGE_FRAGMENT_BIT, builder));
  return *this;
}

ShaderBuilder::ShaderBuildResult ShaderBuilder::Build() const {
  return result_;
}

ShaderStructure ShaderBuilder::CreateShader(
    const VkShaderStageFlagBits stage,
    const DescriptorReferenceBuilder& descriptorReferenceBuilder) {
  DescriptorReferenceBuilder::DescriptorReferences references =
      descriptorReferenceBuilder.Build();
  return {.shaderStage = stage,
          .globalBindings = std::move(references.globalBindings),
          .localBindings = std::move(references.localBindings)};
}
