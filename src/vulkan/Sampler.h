#ifndef VULKAN_SRC_VULKAN_SAMPLER_H
#define VULKAN_SRC_VULKAN_SAMPLER_H

#include <vulkan/vulkan.h>

#include "lifetime_semantics.h"
#include "structures/SamplerCreateInfo.h"

class Sampler {
  friend class DescriptorSet;
public:
  VULKAN_OBJECT_MOVABLE_ROOT(Sampler, device, sampler)

  Sampler() = default;
  Sampler(VkDevice device, SamplerCreateInfoBuilder& infoBuilder);

  Sampler(const Sampler&) = delete;

  ~Sampler();

  Sampler& operator=(const Sampler&) = delete;

private:
  VkDevice device;
  VkSampler sampler = nullptr;
};

#endif // VULKAN_SRC_VULKAN_SAMPLER_H
