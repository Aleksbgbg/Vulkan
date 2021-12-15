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
  SceneComposer(ActorConsumer& actorConsumer, Renderer& renderer,
                sys::Sound& sound, game::Camera& camera);

  MeshHandle LoadMesh(const RenderType renderType,
                      const MeshLoadParams& meshLoadParams);

  CompositionBuilder Actor();
  CompositionBuilder Camera();
  CompositionBuilder ParticleSystem(const ParticleBehaviour behaviour);

 private:
  ActorConsumer& actorConsumer_;
  Renderer& renderer_;
  sys::Sound& sound_;
  game::Camera& camera_;
};

#endif  // VULKAN_SRC_GAME_COMPOSITION_SCENECOMPOSER_H_
