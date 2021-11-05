#include "TextureRegistry.h"

TextureRegistry::TextureRegistry(
    const DescriptorSet& textureSamplerDescriptorSet,
    const Sampler& textureSampler,
    std::vector<DescriptorSet::WriteDescriptorSet>& writeDescriptorSets)
    : textureSamplerDescriptorSet(textureSamplerDescriptorSet),
      textureSampler(textureSampler),
      writeDescriptorSets(writeDescriptorSets) {}

void TextureRegistry::WriteTexture(const ImageView& textureView,
                                   const u32 binding) {
  writeDescriptorSets.push_back(
      std::move(textureSamplerDescriptorSet.CreateImageSamplerWrite(
          textureView, textureSampler, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
          binding)));
}
