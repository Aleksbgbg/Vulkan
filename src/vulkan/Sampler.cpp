#include "Sampler.h"
#include "error.h"

Sampler::Sampler(VkDevice device, SamplerCreateInfoBuilder& infoBuilder) : device(device) {
  PROCEED_ON_VALID_RESULT(vkCreateSampler(device, infoBuilder.Build(), nullptr, &sampler))
}

Sampler::~Sampler() {
  if (sampler != nullptr) {
    vkDestroySampler(device, sampler, nullptr);
  }
}
