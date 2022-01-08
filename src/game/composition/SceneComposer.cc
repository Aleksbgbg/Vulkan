#include "SceneComposer.h"

#include "game/behaviours/CameraController.h"
#include "game/composition/behaviour_utils.h"

SceneComposer::SceneComposer(game::Camera& camera, ActorConsumer& actorConsumer,
                             ActorOwner& actorOwner, Renderer& renderer,
                             sys::Sound& sound)
    : actorKeyGenerator_(),
      camera_(camera),
      spawnDependencies_({.actorConsumer = &actorConsumer,
                          .actorOwner = &actorOwner,
                          .renderer = &renderer,
                          .sound = &sound,
                          .actorKeyGenerator = &actorKeyGenerator_}) {}

MeshHandle SceneComposer::LoadMesh(const RenderType renderType,
                                   const MeshLoadParams& meshLoadParams) {
  return spawnDependencies_.renderer->LoadMesh(renderType, meshLoadParams);
}

CompositionBuilder SceneComposer::Actor() {
  return CompositionBuilder::Actor(spawnDependencies_);
}

CompositionBuilder SceneComposer::Camera() {
  return std::move(
      CompositionBuilder::Actor(spawnDependencies_)
          .Attach(BEHAVIOUR(CameraController, camera_,
                            parent->RetrieveProperty<Transform>())));
}

CompositionBuilder SceneComposer::ParticleSystem(
    const ParticleBehaviour behaviour) {
  return CompositionBuilder::ParticleSystem(spawnDependencies_, behaviour);
}
