#ifndef VULKAN_SRC_GAME_RENDERING_RESOURCES_RESOURCEBINDER_H
#define VULKAN_SRC_GAME_RENDERING_RESOURCES_RESOURCEBINDER_H

#include <vulkan/vulkan.h>

#include <vector>

#include "vulkan/DescriptorSet.h"
#include "vulkan/ImageView.h"
#include "vulkan/Sampler.h"

class ResourceBinder {
 public:
  class ImageSamplerWriter {
   public:
    virtual ~ImageSamplerWriter() = default;

    virtual DescriptorSet::WriteDescriptorSet CreateImageSamplerWrite(
        const DescriptorSet& descriptorSet, const ImageView& imageView,
        const u32 binding) const = 0;
  };

  ResourceBinder(
      const ImageSamplerWriter& imageSamplerWriter,
      const DescriptorSet& textureSamplerDescriptorSet,
      std::vector<DescriptorSet::WriteDescriptorSet>& writeDescriptorSets);

  void BindTexture(const ImageView& textureView, const u32 binding);
  void BindBuffer(const Buffer& buffer, const VkDeviceSize range,
                  const VkDescriptorType descriptorType, const u32 binding);

 private:
  const ImageSamplerWriter& imageSamplerWriter;
  const DescriptorSet& textureSamplerDescriptorSet;
  std::vector<DescriptorSet::WriteDescriptorSet>& writeDescriptorSets;
};

#endif  // VULKAN_SRC_GAME_RENDERING_RESOURCES_RESOURCEBINDER_H
