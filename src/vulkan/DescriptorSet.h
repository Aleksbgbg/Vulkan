#ifndef VULKAN_SRC_VULKAN_DESCRIPTORSET_H
#define VULKAN_SRC_VULKAN_DESCRIPTORSET_H

#include <vulkan/vulkan.h>
#include "Buffer.h"
#include "ImageView.h"
#include "Sampler.h"
#include "vulkan/structures/WriteDescriptorSet.h"

class DescriptorSet {
  friend class VirtualDevice;
  friend class CommandBuffer;
public:
  struct WriteDescriptorSetBuild {
    WriteDescriptorSetBuilder builder;
    union {
      VkDescriptorImageInfo imageInfo;
      VkDescriptorBufferInfo bufferInfo;
    } info;
  };

  DescriptorSet() = default;
  explicit DescriptorSet(VkDescriptorSet descriptorSet);

  DescriptorSet(const DescriptorSet&) = delete;
  DescriptorSet(DescriptorSet&&) = default;
  DescriptorSet& operator=(const DescriptorSet&) = delete;
  DescriptorSet& operator=(DescriptorSet&&) = default;

  WriteDescriptorSetBuild CreateBufferWrite(const Buffer& buffer) const;
  WriteDescriptorSetBuild CreateImageSamplerWrite(
      const ImageView& samplerImageView, const Sampler& sampler, const VkImageLayout imageLayout) const;

private:
  VkDescriptorSet descriptorSet = nullptr;
};

#endif // VULKAN_SRC_VULKAN_DESCRIPTORSET_H
