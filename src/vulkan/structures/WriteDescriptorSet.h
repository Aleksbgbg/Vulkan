#ifndef VULKAN_SRC_VULKAN_STRUCTURES_WRITEDESCRIPTORSET_H
#define VULKAN_SRC_VULKAN_STRUCTURES_WRITEDESCRIPTORSET_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"
#include "DescriptorBufferInfo.h"
#include "DescriptorImageInfo.h"

STRUCTURE_BUILDER(WriteDescriptorSetBuilder, VkWriteDescriptorSet, VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET)
  STRUCTURE_SETTER(WriteDescriptorSetBuilder, DstSet, VkDescriptorSet, dstSet)
  STRUCTURE_SETTER(WriteDescriptorSetBuilder, DstBinding, u32, dstBinding)
  STRUCTURE_SETTER(WriteDescriptorSetBuilder, DstArrayElement, u32, dstArrayElement)
  STRUCTURE_SETTER(WriteDescriptorSetBuilder, DescriptorCount, u32, descriptorCount)
  STRUCTURE_SETTER(WriteDescriptorSetBuilder, DescriptorType, VkDescriptorType, descriptorType)
  STRUCTURE_SETTER(WriteDescriptorSetBuilder, PImageInfo, const VkDescriptorImageInfo*, pImageInfo)
  STRUCTURE_SETTER_BUILDER(WriteDescriptorSetBuilder, PImageInfo, DescriptorImageInfoBuilder, pImageInfo)
  STRUCTURE_SETTER(WriteDescriptorSetBuilder, PBufferInfo, const VkDescriptorBufferInfo*, pBufferInfo)
  STRUCTURE_SETTER_BUILDER(WriteDescriptorSetBuilder, PBufferInfo, DescriptorBufferInfoBuilder, pBufferInfo)
  STRUCTURE_SETTER(WriteDescriptorSetBuilder, PTexelBufferView, const VkBufferView*, pTexelBufferView)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_WRITEDESCRIPTORSET_H
