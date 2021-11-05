#ifndef VULKAN_SRC_GAME_RENDERING_RESOURCES_TEXTUREREGISTRY_H
#define VULKAN_SRC_GAME_RENDERING_RESOURCES_TEXTUREREGISTRY_H

#include <vector>

#include "vulkan/DescriptorSet.h"
#include "vulkan/ImageView.h"
#include "vulkan/Sampler.h"

class TextureRegistry {
 public:
  TextureRegistry(
      const DescriptorSet& textureSamplerDescriptorSet,
      const Sampler& textureSampler,
      std::vector<DescriptorSet::WriteDescriptorSet>& writeDescriptorSets);

  void WriteTexture(const ImageView& textureView, const u32 binding);

 private:
  const DescriptorSet& textureSamplerDescriptorSet;
  const Sampler& textureSampler;
  std::vector<DescriptorSet::WriteDescriptorSet>& writeDescriptorSets;
};

#endif  // VULKAN_SRC_GAME_RENDERING_RESOURCES_TEXTUREREGISTRY_H
