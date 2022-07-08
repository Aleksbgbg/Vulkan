#ifndef SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_PARTICLE_H_
#define SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_PARTICLE_H_

#include "core/types.h"
#include "util/include/glm.h"

struct Particle {
  alignas(16) glm::mat4 baseTransform;
  alignas(16) glm::vec3 position;
  alignas(4) float velocity;
  alignas(4) float totalLife;
  alignas(4) float timeToLive;
  alignas(4) u32 isRendering;
};

#endif  // SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_PARTICLE_H_
