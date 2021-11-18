#include "ResourceBinder.h"

ResourceBinder::ResourceBinder(
    const ImageSamplerWriter& imageSamplerWriter,
    const DescriptorSet& textureSamplerDescriptorSet,
    std::vector<DescriptorSet::WriteDescriptorSet>& writeDescriptorSets)
    : imageSamplerWriter(imageSamplerWriter),
      textureSamplerDescriptorSet(textureSamplerDescriptorSet),
      writeDescriptorSets(writeDescriptorSets) {}

void ResourceBinder::BindTexture(const ImageView& textureView,
                                 const u32 binding) {
  writeDescriptorSets.push_back(
      std::move(imageSamplerWriter.CreateImageSamplerWrite(
          textureSamplerDescriptorSet, textureView, binding)));
}

void ResourceBinder::BindBuffer(const Buffer& buffer, const VkDeviceSize range,
                                const VkDescriptorType descriptorType,
                                const u32 binding) {
  writeDescriptorSets.push_back(
      std::move(textureSamplerDescriptorSet.CreateBufferWrite(
          buffer, range, descriptorType, binding)));
}
