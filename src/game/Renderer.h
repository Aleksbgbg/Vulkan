#ifndef VULKAN_SRC_GAME_RENDERER_H_
#define VULKAN_SRC_GAME_RENDERER_H_

#include "DrawList.h"
#include "LightSource.h"
#include "Transformable.h"
#include "Visible.h"
#include "game/actor/resource/Resource.h"
#include "game/composition/MeshHandle.h"
#include "game/composition/MeshLoadParams.h"
#include "game/composition/ParticleBehaviour.h"
#include "game/composition/RenderType.h"

class Renderer {
 public:
  virtual ~Renderer() = default;

  virtual MeshHandle LoadMesh(const RenderType renderType,
                              const MeshLoadParams& meshLoadParams) = 0;

  struct LightSourceInfo {
    LightSource lightSource;
    const Transformable& transformable;
  };
  virtual std::unique_ptr<Resource> SpawnLightSource(
      const LightSourceInfo& lightSourceInfo) = 0;

  struct RenderInfo {
    MeshHandle meshHandle;
    const Transformable& transformable;
    const Visible& visible;
  };
  virtual std::unique_ptr<Resource> SpawnRenderable(
      const RenderInfo& renderInfo) = 0;

  struct ParticleSystemInfo {
    MeshHandle meshHandle;
    ParticleBehaviour particleBehaviour;
    const Transformable& transformable;
    const Visible& visible;
    glm::vec3 spawnRegionLow;
    glm::vec3 spawnRegionHigh;
  };
  virtual std::unique_ptr<Resource> SpawnParticleSystem(
      const ParticleSystemInfo& particleSystemInfo) = 0;

  struct UiInfo {
    DrawList* drawList;
    const Visible& visible;
  };
  virtual std::unique_ptr<Resource> SpawnUi(const UiInfo& uiInfo) = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERER_H_
