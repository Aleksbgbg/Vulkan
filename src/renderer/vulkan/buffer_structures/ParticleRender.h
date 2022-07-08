#ifndef SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_PARTICLERENDER_H_
#define SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_PARTICLERENDER_H_

#include "util/include/glm.h"

struct ParticleRender {
  alignas(16) glm::mat4 transform;
  alignas(4) float fractionOfLife;
};

#endif  // SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_PARTICLERENDER_H_
