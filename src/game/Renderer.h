#ifndef VULKAN_SRC_GAME_RENDERER_H_
#define VULKAN_SRC_GAME_RENDERER_H_

#include "SpawnControllable.h"
#include "Transformable.h"
#include "game/actor/Resource.h"
#include "game/composition/MeshHandle.h"
#include "game/composition/MeshLoadParams.h"
#include "game/composition/ParticleBehaviour.h"
#include "game/composition/RenderType.h"

class Renderer {
 public:
  virtual ~Renderer() = default;

  virtual MeshHandle LoadMesh(const RenderType renderType,
                              const MeshLoadParams& meshLoadParams) = 0;

  struct RenderInfo {
    MeshHandle meshHandle;
    const Transformable* transformable;
  };
  virtual std::unique_ptr<Resource> SpawnRenderable(RenderInfo renderInfo) = 0;

  struct ParticleSystemInfo {
    MeshHandle meshHandle;
    ParticleBehaviour particleBehaviour;
    const Transformable* transformable;
    const SpawnControllable* spawnControllable;
    glm::vec3 spawnRegionLow;
    glm::vec3 spawnRegionHigh;
  };
  virtual std::unique_ptr<Resource> SpawnParticleSystem(
      const ParticleSystemInfo& particleSystemInfo) = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERER_H_
