#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_DESCRIPTORSETLAYOUTCREATEINFO_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_DESCRIPTORSETLAYOUTCREATEINFO_H_

#include <vulkan/vulkan.h>

#include "DescriptorSetLayoutBinding.h"
#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(DescriptorSetLayoutCreateInfoBuilder,
                  VkDescriptorSetLayoutCreateInfo,
                  VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO)
STRUCTURE_SETTER(Flags, const VkDescriptorSetLayoutCreateFlags, flags)
STRUCTURE_SETTER(BindingCount, const u32, bindingCount)
STRUCTURE_SETTER(PBindings, const VkDescriptorSetLayoutBinding*, pBindings)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PBindings,
                                      DescriptorSetLayoutBindingBuilder,
                                      pBindings)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_DESCRIPTORSETLAYOUTCREATEINFO_H_
