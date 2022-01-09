#ifndef VULKAN_SRC_RENDERER_VULKAN_API_SAMPLER_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_SAMPLER_H_

#include <vulkan/vulkan.h>

#include "renderer/vulkan/api/structures/SamplerCreateInfo.h"

namespace vk {

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

}  // namespace vk

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_SAMPLER_H_
