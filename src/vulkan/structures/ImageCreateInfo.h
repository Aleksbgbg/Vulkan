#ifndef VULKAN_SRC_VULKAN_STRUCTURES_IMAGECREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_IMAGECREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"
#include "Extent3D.h"

STRUCTURE_BUILDER(ImageCreateInfoBuilder, VkImageCreateInfo, VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO)
  STRUCTURE_SETTER(ImageCreateInfoBuilder, Flags, const VkImageCreateFlags, flags)
  STRUCTURE_SETTER(ImageCreateInfoBuilder, ImageType, const VkImageType, imageType)
  STRUCTURE_SETTER(ImageCreateInfoBuilder, Format, const VkFormat, format)
  STRUCTURE_SETTER(ImageCreateInfoBuilder, Extent, const VkExtent3D, extent)
  STRUCTURE_SETTER_BUILDER_OBJECT(ImageCreateInfoBuilder, Extent, Extent3DBuilder, extent)
  STRUCTURE_SETTER(ImageCreateInfoBuilder, MipLevels, const u32, mipLevels)
  STRUCTURE_SETTER(ImageCreateInfoBuilder, ArrayLayers, const u32, arrayLayers)
  STRUCTURE_SETTER(ImageCreateInfoBuilder, Samples, const VkSampleCountFlagBits, samples)
  STRUCTURE_SETTER(ImageCreateInfoBuilder, Tiling, const VkImageTiling, tiling)
  STRUCTURE_SETTER(ImageCreateInfoBuilder, Usage, const VkImageUsageFlags, usage)
  STRUCTURE_SETTER(ImageCreateInfoBuilder, SharingMode, const VkSharingMode, sharingMode)
  STRUCTURE_SETTER(ImageCreateInfoBuilder, QueueFamilyIndexCount, const u32, queueFamilyIndexCount)
  STRUCTURE_SETTER(ImageCreateInfoBuilder, PQueueFamilyIndices, const u32*, pQueueFamilyIndices)
  STRUCTURE_SETTER(ImageCreateInfoBuilder, InitialLayout, const VkImageLayout, initialLayout)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_IMAGECREATEINFO_H
