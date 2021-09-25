#ifndef VULKAN_SRC_VULKAN_STRUCTURES_SAMPLERCREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_SAMPLERCREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(SamplerCreateInfoBuilder, VkSamplerCreateInfo, VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO)
  STRUCTURE_SETTER(SamplerCreateInfoBuilder, Flags, VkSamplerCreateFlags, flags)
  STRUCTURE_SETTER(SamplerCreateInfoBuilder, MagFilter, VkFilter, magFilter)
  STRUCTURE_SETTER(SamplerCreateInfoBuilder, MinFilter, VkFilter, minFilter)
  STRUCTURE_SETTER(SamplerCreateInfoBuilder, MipmapMode, VkSamplerMipmapMode, mipmapMode)
  STRUCTURE_SETTER(SamplerCreateInfoBuilder, AddressModeU, VkSamplerAddressMode, addressModeU)
  STRUCTURE_SETTER(SamplerCreateInfoBuilder, AddressModeV, VkSamplerAddressMode, addressModeV)
  STRUCTURE_SETTER(SamplerCreateInfoBuilder, AddressModeW, VkSamplerAddressMode, addressModeW)
  STRUCTURE_SETTER(SamplerCreateInfoBuilder, MipLodBias, float, mipLodBias)
  STRUCTURE_SETTER(SamplerCreateInfoBuilder, AnisotropyEnable, VkBool32, anisotropyEnable)
  STRUCTURE_SETTER(SamplerCreateInfoBuilder, MaxAnisotropy, float, maxAnisotropy)
  STRUCTURE_SETTER(SamplerCreateInfoBuilder, CompareEnable, VkBool32, compareEnable)
  STRUCTURE_SETTER(SamplerCreateInfoBuilder, CompareOp, VkCompareOp, compareOp)
  STRUCTURE_SETTER(SamplerCreateInfoBuilder, MinLod, float, minLod)
  STRUCTURE_SETTER(SamplerCreateInfoBuilder, MaxLod, float, maxLod)
  STRUCTURE_SETTER(SamplerCreateInfoBuilder, BorderColor, VkBorderColor, borderColor)
  STRUCTURE_SETTER(SamplerCreateInfoBuilder, UnnormalizedCoordinates, VkBool32, unnormalizedCoordinates)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_SAMPLERCREATEINFO_H
