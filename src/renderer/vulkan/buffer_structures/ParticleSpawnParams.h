#ifndef VULKAN_SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_PARTICLESPAWNPARAMS_H_
#define VULKAN_SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_PARTICLESPAWNPARAMS_H_

#include "util/include/glm.h"
#include "util/types.h"

struct ParticleSpawnParams {
  alignas(4) u32 randomSeed;
  alignas(4) u32 enableRespawn;
  alignas(16) glm::mat4 baseTransform;
  alignas(16) glm::vec3 spawnRegionLow;
  alignas(16) glm::vec3 spawnRegionHigh;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_PARTICLESPAWNPARAMS_H_
