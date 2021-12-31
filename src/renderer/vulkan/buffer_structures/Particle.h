#ifndef VULKAN_SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_PARTICLE_H_
#define VULKAN_SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_PARTICLE_H_

#include "util/include/glm.h"
#include "util/types.h"

struct Particle {
  alignas(16) glm::mat4 baseTransform;
  alignas(16) glm::vec3 position;
  alignas(4) float velocity;
  alignas(4) float totalLife;
  alignas(4) float timeToLive;
  alignas(4) u32 isRendering;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_PARTICLE_H_
