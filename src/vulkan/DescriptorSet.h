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
  class WriteDescriptorSet {
    friend class DescriptorSet;
  public:
    WriteDescriptorSetBuilder& Builder() {
      return writeBuilder;
    }

    operator VkWriteDescriptorSet() const {
      return writeBuilder.BuildObject();
    }

  private:
    WriteDescriptorSetBuilder writeBuilder;

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

  void CreateBufferWrite(const Buffer& buffer, DescriptorSet::WriteDescriptorSet& writeDescriptorSet) const;
  void CreateImageSamplerWrite(
      const ImageView& samplerImageView,
      const Sampler& sampler,
      const VkImageLayout imageLayout,
      DescriptorSet::WriteDescriptorSet& writeDescriptorSet) const;

private:
  VkDescriptorSet descriptorSet = nullptr;
};

#endif // VULKAN_SRC_VULKAN_DESCRIPTORSET_H
