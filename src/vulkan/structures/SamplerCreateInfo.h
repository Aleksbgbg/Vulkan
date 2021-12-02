#ifndef VULKAN_SRC_VULKAN_STRUCTURES_SAMPLERCREATEINFO_H_
#define VULKAN_SRC_VULKAN_STRUCTURES_SAMPLERCREATEINFO_H_

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(SamplerCreateInfoBuilder, VkSamplerCreateInfo,
                  VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO)
STRUCTURE_SETTER(Flags, VkSamplerCreateFlags, flags)
STRUCTURE_SETTER(MagFilter, VkFilter, magFilter)
STRUCTURE_SETTER(MinFilter, VkFilter, minFilter)
STRUCTURE_SETTER(MipmapMode, VkSamplerMipmapMode, mipmapMode)
STRUCTURE_SETTER(AddressModeU, VkSamplerAddressMode, addressModeU)
STRUCTURE_SETTER(AddressModeV, VkSamplerAddressMode, addressModeV)
STRUCTURE_SETTER(AddressModeW, VkSamplerAddressMode, addressModeW)
STRUCTURE_SETTER(MipLodBias, float, mipLodBias)
STRUCTURE_SETTER(AnisotropyEnable, VkBool32, anisotropyEnable)
STRUCTURE_SETTER(MaxAnisotropy, float, maxAnisotropy)
STRUCTURE_SETTER(CompareEnable, VkBool32, compareEnable)
STRUCTURE_SETTER(CompareOp, VkCompareOp, compareOp)
STRUCTURE_SETTER(MinLod, float, minLod)
STRUCTURE_SETTER(MaxLod, float, maxLod)
STRUCTURE_SETTER(BorderColor, VkBorderColor, borderColor)
STRUCTURE_SETTER(UnnormalizedCoordinates, VkBool32, unnormalizedCoordinates)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_SAMPLERCREATEINFO_H_
