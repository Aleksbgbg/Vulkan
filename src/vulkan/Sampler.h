#ifndef VULKAN_SRC_VULKAN_SAMPLER_H_
#define VULKAN_SRC_VULKAN_SAMPLER_H_

#include <vulkan/vulkan.h>

#include "structures/SamplerCreateInfo.h"

class Sampler {
  friend class DescriptorSet;

 public:
  Sampler();
  Sampler(VkDevice device, SamplerCreateInfoBuilder& infoBuilder);

  Sampler(const Sampler&) = delete;
  Sampler(Sampler&& other) noexcept;

  ~Sampler();

  Sampler& operator=(const Sampler&) = delete;
  Sampler& operator=(Sampler&& other) noexcept;

 private:
  VkDevice device;
  VkSampler sampler;
};

#endif  // VULKAN_SRC_VULKAN_SAMPLER_H_
