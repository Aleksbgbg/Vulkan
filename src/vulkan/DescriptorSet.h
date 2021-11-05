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
    //    WriteDescriptorSet();
    //
    //    WriteDescriptorSet(const WriteDescriptorSet& other) noexcept;
    //    WriteDescriptorSet(WriteDescriptorSet&& other) noexcept;
    //
    //    WriteDescriptorSet& operator=(const WriteDescriptorSet& other)
    //    noexcept; WriteDescriptorSet& operator=(WriteDescriptorSet&& other)
    //    noexcept;

    operator VkWriteDescriptorSet() const;

   private:
    //    enum class Type {
    //      None,
    //      Buffer,
    //      Image
    //    };
    //
    //    Type type;

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

  void CreateBufferWrite(
      const Buffer& buffer, const VkDeviceSize range,
      const VkDescriptorType descriptorType,
      DescriptorSet::WriteDescriptorSet& writeDescriptorSet) const;
  void CreateBufferWrite(
      const Buffer& buffer, const VkDeviceSize range,
      const VkDescriptorType descriptorType, const u32 binding,
      DescriptorSet::WriteDescriptorSet& writeDescriptorSet) const;
  void CreateImageSamplerWrite(
      const ImageView& samplerImageView, const Sampler& sampler,
      const VkImageLayout imageLayout,
      DescriptorSet::WriteDescriptorSet& writeDescriptorSet) const;
  void CreateImageSamplerWrite(
      const ImageView& samplerImageView, const Sampler& sampler,
      const VkImageLayout imageLayout, const u32 binding,
      DescriptorSet::WriteDescriptorSet& writeDescriptorSet) const;

 private:
  VkDescriptorSet descriptorSet = nullptr;
};

#endif  // VULKAN_SRC_VULKAN_DESCRIPTORSET_H
