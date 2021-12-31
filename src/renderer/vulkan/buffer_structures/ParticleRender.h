#ifndef VULKAN_SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_PARTICLERENDER_H_
#define VULKAN_SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_PARTICLERENDER_H_

#include "util/include/glm.h"

struct ParticleRender {
  alignas(16) glm::mat4 transform;
  alignas(4) float fractionOfLife;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_PARTICLERENDER_H_
