#include "TextureRegistry.h"

TextureRegistry::TextureRegistry(
    const DescriptorSet& textureSamplerDescriptorSet,
    const Sampler& textureSampler,
    DescriptorSet::WriteDescriptorSet& textureSamplerWrite)
    : textureSamplerDescriptorSet(textureSamplerDescriptorSet),
      textureSampler(textureSampler),
      textureSamplerWrite(textureSamplerWrite) {}

void TextureRegistry::WriteTexture(const ImageView& textureView) {
  textureSamplerDescriptorSet.CreateImageSamplerWrite(
      textureView, textureSampler, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      textureSamplerWrite);
}
