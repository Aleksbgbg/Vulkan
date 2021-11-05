#ifndef VULKAN_SRC_VULKANCONTEXT_H
#define VULKAN_SRC_VULKANCONTEXT_H

#include <vulkan/vulkan.h>

#include "vulkan/PipelineCache.h"
#include "vulkan/SubpassReference.h"
#include "vulkan/VirtualDevice.h"

struct VulkanContext {
  const VirtualDevice* virtualDevice;
  const PipelineCache* pipelineCache;
  const SubpassReference* subpassReference;
  const Sampler* sampler;

  VkSampleCountFlagBits samples;
};

#endif  // VULKAN_SRC_VULKANCONTEXT_H
