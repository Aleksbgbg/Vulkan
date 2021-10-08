#ifndef VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORPOOLSIZE_H
#define VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORPOOLSIZE_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER_NO_TYPE(DescriptorPoolSizeBuilder, VkDescriptorPoolSize)
STRUCTURE_SETTER(DescriptorPoolSizeBuilder, Type, VkDescriptorType, type)
STRUCTURE_SETTER(DescriptorPoolSizeBuilder, DescriptorCount, u32,
                 descriptorCount)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORPOOLSIZE_H
