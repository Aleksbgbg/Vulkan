#ifndef VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORPOOLCREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORPOOLCREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "DescriptorPoolSize.h"
#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER(DescriptorPoolCreateInfoBuilder, VkDescriptorPoolCreateInfo,
                  VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO)
STRUCTURE_SETTER(DescriptorPoolCreateInfoBuilder, Flags,
                 VkDescriptorPoolCreateFlags, flags)
STRUCTURE_SETTER(DescriptorPoolCreateInfoBuilder, MaxSets, u32, maxSets)
STRUCTURE_SETTER(DescriptorPoolCreateInfoBuilder, PoolSizeCount, u32,
                 poolSizeCount)
STRUCTURE_SETTER(DescriptorPoolCreateInfoBuilder, PPoolSizes,
                 const VkDescriptorPoolSize*, pPoolSizes)
STRUCTURE_SETTER_BUILDER(DescriptorPoolCreateInfoBuilder, PPoolSizes,
                         DescriptorPoolSizeBuilder, pPoolSizes)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORPOOLCREATEINFO_H
