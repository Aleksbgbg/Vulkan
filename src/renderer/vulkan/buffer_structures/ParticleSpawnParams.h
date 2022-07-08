#ifndef SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_PARTICLESPAWNPARAMS_H_
#define SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_PARTICLESPAWNPARAMS_H_

#include "core/types.h"
#include "util/include/glm.h"

struct ParticleSpawnParams {
  alignas(4) u32 randomSeed;
  alignas(4) u32 enableRespawn;
  alignas(16) glm::mat4 baseTransform;
  alignas(16) glm::vec3 spawnRegionLow;
  alignas(16) glm::vec3 spawnRegionHigh;
};

#endif  // SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_PARTICLESPAWNPARAMS_H_
