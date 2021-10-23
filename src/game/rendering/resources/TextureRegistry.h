#ifndef VULKAN_SRC_GAME_RENDERING_RESOURCES_TEXTUREREGISTRY_H
#define VULKAN_SRC_GAME_RENDERING_RESOURCES_TEXTUREREGISTRY_H

#include "vulkan/DescriptorSet.h"
#include "vulkan/ImageView.h"
#include "vulkan/Sampler.h"

class TextureRegistry {
 public:
  TextureRegistry(const DescriptorSet& textureSamplerDescriptorSet,
                  const Sampler& textureSampler,
                  DescriptorSet::WriteDescriptorSet& textureSamplerWrite);

  void WriteTexture(const ImageView& textureView);

 private:
  const DescriptorSet& textureSamplerDescriptorSet;
  const Sampler& textureSampler;
  DescriptorSet::WriteDescriptorSet& textureSamplerWrite;
};

#endif  // VULKAN_SRC_GAME_RENDERING_RESOURCES_TEXTUREREGISTRY_H
