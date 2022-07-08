#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_IMAGECREATEINFO_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_IMAGECREATEINFO_H_

#include <vulkan/vulkan.h>

#include "Extent3D.h"
#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(ImageCreateInfoBuilder, VkImageCreateInfo,
                  VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO)
STRUCTURE_SETTER(Flags, const VkImageCreateFlags, flags)
STRUCTURE_SETTER(ImageType, const VkImageType, imageType)
STRUCTURE_SETTER(Format, const VkFormat, format)
STRUCTURE_SETTER(Extent, const VkExtent3D, extent)
STRUCTURE_SETTER(MipLevels, const u32, mipLevels)
STRUCTURE_SETTER(ArrayLayers, const u32, arrayLayers)
STRUCTURE_SETTER(Samples, const VkSampleCountFlagBits, samples)
STRUCTURE_SETTER(Tiling, const VkImageTiling, tiling)
STRUCTURE_SETTER(Usage, const VkImageUsageFlags, usage)
STRUCTURE_SETTER(SharingMode, const VkSharingMode, sharingMode)
STRUCTURE_SETTER(QueueFamilyIndexCount, const u32, queueFamilyIndexCount)
STRUCTURE_SETTER(PQueueFamilyIndices, const u32*, pQueueFamilyIndices)
STRUCTURE_SETTER(InitialLayout, const VkImageLayout, initialLayout)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_IMAGECREATEINFO_H_
