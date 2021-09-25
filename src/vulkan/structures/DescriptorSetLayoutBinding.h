#ifndef VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORSETLAYOUTBINDING_H
#define VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORSETLAYOUTBINDING_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"

STRUCTURE_BUILDER_NO_TYPE(DescriptorSetLayoutBindingBuilder, VkDescriptorSetLayoutBinding)
  STRUCTURE_SETTER(DescriptorSetLayoutBindingBuilder, Binding, const u32, binding)
  STRUCTURE_SETTER(DescriptorSetLayoutBindingBuilder, DescriptorType, const VkDescriptorType, descriptorType)
  STRUCTURE_SETTER(DescriptorSetLayoutBindingBuilder, DescriptorCount, const u32, descriptorCount)
  STRUCTURE_SETTER(DescriptorSetLayoutBindingBuilder, StageFlags, const VkShaderStageFlags, stageFlags)
  STRUCTURE_SETTER(DescriptorSetLayoutBindingBuilder, PImmutableSamplers, const VkSampler*, pImmutableSamplers)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORSETLAYOUTBINDING_H
