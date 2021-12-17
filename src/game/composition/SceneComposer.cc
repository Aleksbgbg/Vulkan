#include "SceneComposer.h"

#include "game/behaviours/CameraController.h"

SceneComposer::SceneComposer(game::Camera& camera,
                             const SpawnDependencies spawnDependencies)
    : camera_(camera), spawnDependencies_(spawnDependencies) {}

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
