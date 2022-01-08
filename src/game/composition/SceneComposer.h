#ifndef VULKAN_SRC_GAME_COMPOSITION_SCENECOMPOSER_H_
#define VULKAN_SRC_GAME_COMPOSITION_SCENECOMPOSER_H_

#include "ActorConsumer.h"
#include "CompositionBuilder.h"
#include "MeshHandle.h"
#include "MeshLoadParams.h"
#include "ParticleBehaviour.h"
#include "game/Camera.h"
#include "game/Renderer.h"

class SceneComposer {
 public:
  SceneComposer(game::Camera& camera, ActorConsumer& actorConsumer,
                ActorOwner& actorOwner, Renderer& renderer, sys::Sound& sound);

  MeshHandle LoadMesh(const RenderType renderType,
                      const MeshLoadParams& meshLoadParams);

  CompositionBuilder Actor();
  CompositionBuilder Camera();
  CompositionBuilder ParticleSystem(const ParticleBehaviour behaviour);

 private:
  ActorKeyGenerator actorKeyGenerator_;
  game::Camera& camera_;
  SpawnDependencies spawnDependencies_;
};

#endif  // VULKAN_SRC_GAME_COMPOSITION_SCENECOMPOSER_H_
