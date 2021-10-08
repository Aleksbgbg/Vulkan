#ifndef VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORBUFFERINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORBUFFERINFO_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER_NO_TYPE(DescriptorBufferInfoBuilder, VkDescriptorBufferInfo)
STRUCTURE_SETTER(DescriptorBufferInfoBuilder, Buffer, VkBuffer, buffer)
STRUCTURE_SETTER(DescriptorBufferInfoBuilder, Offset, VkDeviceSize, offset)
STRUCTURE_SETTER(DescriptorBufferInfoBuilder, Range, VkDeviceSize, range)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORBUFFERINFO_H
