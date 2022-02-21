#ifndef VULKAN_SRC_ENGINE_COMPOSITION_RENDERER_H_
#define VULKAN_SRC_ENGINE_COMPOSITION_RENDERER_H_

#include "LightSource.h"
#include "engine/composition/MeshHandle.h"
#include "engine/composition/MeshLoadParams.h"
#include "engine/composition/ParticleBehaviour.h"
#include "engine/composition/RenderType.h"
#include "engine/property/traits/Transformable.h"
#include "engine/property/traits/Visible.h"
#include "engine/resource/Resource.h"
#include "engine/ui/DrawList.h"

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

#endif  // VULKAN_SRC_ENGINE_COMPOSITION_RENDERER_H_
