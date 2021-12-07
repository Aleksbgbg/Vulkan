#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_IMAGEMEMORYBARRIER_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_IMAGEMEMORYBARRIER_H_

#include <vulkan/vulkan.h>

#include "ImageSubresourceRange.h"
#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(ImageMemoryBarrierBuilder, VkImageMemoryBarrier,
                  VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER)
STRUCTURE_SETTER(SrcAccessMask, VkAccessFlags, srcAccessMask)
STRUCTURE_SETTER(DstAccessMask, VkAccessFlags, dstAccessMask)
STRUCTURE_SETTER(OldLayout, VkImageLayout, oldLayout)
STRUCTURE_SETTER(NewLayout, VkImageLayout, newLayout)
STRUCTURE_SETTER(SrcQueueFamilyIndex, u32, srcQueueFamilyIndex)
STRUCTURE_SETTER(DstQueueFamilyIndex, u32, dstQueueFamilyIndex)
STRUCTURE_SETTER(Image, VkImage, image)
STRUCTURE_SETTER(SubresourceRange, VkImageSubresourceRange, subresourceRange)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_IMAGEMEMORYBARRIER_H_
