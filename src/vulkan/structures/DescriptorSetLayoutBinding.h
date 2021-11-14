#ifndef VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORSETLAYOUTBINDING_H
#define VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORSETLAYOUTBINDING_H

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER_NO_STYPE(DescriptorSetLayoutBindingBuilder,
                           VkDescriptorSetLayoutBinding)
STRUCTURE_SETTER(Binding, const u32, binding)
STRUCTURE_SETTER(DescriptorType, const VkDescriptorType, descriptorType)
STRUCTURE_SETTER(DescriptorCount, const u32, descriptorCount)
STRUCTURE_SETTER(StageFlags, const VkShaderStageFlags, stageFlags)
STRUCTURE_SETTER(PImmutableSamplers, const VkSampler*, pImmutableSamplers)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORSETLAYOUTBINDING_H
