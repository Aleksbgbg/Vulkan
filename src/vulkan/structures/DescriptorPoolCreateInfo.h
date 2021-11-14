#ifndef VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORPOOLCREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORPOOLCREATEINFO_H

#include <vulkan/vulkan.h>

#include "DescriptorPoolSize.h"
#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(DescriptorPoolCreateInfoBuilder, VkDescriptorPoolCreateInfo,
                  VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO)
STRUCTURE_SETTER(Flags, VkDescriptorPoolCreateFlags, flags)
STRUCTURE_SETTER(MaxSets, u32, maxSets)
STRUCTURE_SETTER(PoolSizeCount, u32, poolSizeCount)
STRUCTURE_SETTER(PPoolSizes, const VkDescriptorPoolSize*, pPoolSizes)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PPoolSizes, DescriptorPoolSizeBuilder,
                                      pPoolSizes)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORPOOLCREATEINFO_H
