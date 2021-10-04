#ifndef VULKAN_SRC_VULKAN_DESCRIPTORSET_H
#define VULKAN_SRC_VULKAN_DESCRIPTORSET_H

#include <vulkan/vulkan.h>
#include <memory>
#include "Buffer.h"
#include "ImageView.h"
#include "Sampler.h"
#include "vulkan/structures/WriteDescriptorSet.h"

class DescriptorSet {
  friend class VirtualDevice;
  friend class CommandBuffer;
public:
  struct WriteDescriptorSetBuild {
  public:
    WriteDescriptorSetBuilder writeBuilder;
  private:
    friend class DescriptorSet;
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

  std::unique_ptr<DescriptorSet::WriteDescriptorSetBuild> CreateBufferWrite(const Buffer& buffer) const;
  std::unique_ptr<DescriptorSet::WriteDescriptorSetBuild> CreateImageSamplerWrite(
      const ImageView& samplerImageView, const Sampler& sampler, const VkImageLayout imageLayout) const;

private:
  VkDescriptorSet descriptorSet = nullptr;
};

#endif // VULKAN_SRC_VULKAN_DESCRIPTORSET_H
