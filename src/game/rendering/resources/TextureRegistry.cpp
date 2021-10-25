#include "TextureRegistry.h"

TextureRegistry::TextureRegistry(
    const DescriptorSet& textureSamplerDescriptorSet,
    const Sampler& textureSampler,
    std::vector<std::unique_ptr<DescriptorSet::WriteDescriptorSet>>&
        writeDescriptorSets)
    : textureSamplerDescriptorSet(textureSamplerDescriptorSet),
      textureSampler(textureSampler),
      writeDescriptorSets(writeDescriptorSets) {}

void TextureRegistry::WriteTexture(const ImageView& textureView,
                                   const u32 binding) {
  writeDescriptorSets.emplace_back(
      std::make_unique<DescriptorSet::WriteDescriptorSet>());
  textureSamplerDescriptorSet.CreateImageSamplerWrite(
      textureView, textureSampler, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      binding, *writeDescriptorSets[writeDescriptorSets.size() - 1]);
}
