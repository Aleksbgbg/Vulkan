#ifndef VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORSETLAYOUTCREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORSETLAYOUTCREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "DescriptorSetLayoutBinding.h"
#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER(DescriptorSetLayoutCreateInfoBuilder,
                  VkDescriptorSetLayoutCreateInfo,
                  VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO)
STRUCTURE_SETTER(DescriptorSetLayoutCreateInfoBuilder, Flags,
                 const VkDescriptorSetLayoutCreateFlags, flags)
STRUCTURE_SETTER(DescriptorSetLayoutCreateInfoBuilder, BindingCount, const u32,
                 bindingCount)
STRUCTURE_SETTER(DescriptorSetLayoutCreateInfoBuilder, PBindings,
                 const VkDescriptorSetLayoutBinding*, pBindings)
STRUCTURE_SETTER_BUILDER(DescriptorSetLayoutCreateInfoBuilder, PBindings,
                         DescriptorSetLayoutBindingBuilder, pBindings)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORSETLAYOUTCREATEINFO_H
