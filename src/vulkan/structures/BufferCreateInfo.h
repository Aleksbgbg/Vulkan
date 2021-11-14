#ifndef VULKAN_SRC_VULKAN_STRUCTURES_BUFFERCREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_BUFFERCREATEINFO_H

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(BufferCreateInfoBuilder, VkBufferCreateInfo,
                  VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO)
STRUCTURE_SETTER(Size, const VkDeviceSize, size)
STRUCTURE_SETTER(Usage, const VkBufferUsageFlags, usage)
STRUCTURE_SETTER(SharingMode, const VkSharingMode, sharingMode)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_BUFFERCREATEINFO_H
