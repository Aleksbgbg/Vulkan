#include "Scene.h"

#include "game/behaviours/BackgroundMusic.h"
#include "game/behaviours/ExhaustParticleController.h"
#include "game/behaviours/ForwardMovement.h"
#include "game/behaviours/PlayerMovement.h"
#include "game/behaviours/SunMovement.h"
#include "game/composition/ParticleBehaviour.h"
#include "game/composition/SceneComposer.h"
#include "general/math/math.h"
#include "util/filenames.h"

Scene::Scene(Renderer& renderer, sys::Sound& sound, game::Camera& camera)
    : actors_() {
  constexpr float ShipMinX = -1.7f;
  constexpr float ShipMaxX = 1.7f;
  constexpr float ShipMinY = -0.8f;
  constexpr float ShipMaxY = 0.8f;
  constexpr float ShipMinZ = -2.4f;
  constexpr float ShipMaxZ = 2.8f;

  constexpr float ModelMaxX = 34.0f;
  constexpr float ModelMaxY = 16.0f;
  constexpr float ModelMaxZ = 56.0f;

  // Ship exhaust calculation
  constexpr float x1 =
      AffineTransform(6.0f, 0.0f, ModelMaxX, ShipMinX, ShipMaxX);
  constexpr float y1 =
      AffineTransform(5.0f, 0.0f, ModelMaxY, ShipMinY, ShipMaxY);
  constexpr float z1 =
      AffineTransform(ModelMaxZ - 56.0f, 0.0f, ModelMaxZ, ShipMinZ, ShipMaxZ);
  constexpr float x2 =
      AffineTransform(27.0f, 0.0f, ModelMaxX, ShipMinX, ShipMaxX);
  constexpr float y2 =
      AffineTransform(7.0f, 0.0f, ModelMaxY, ShipMinY, ShipMaxY);
  constexpr float z2 =
      AffineTransform(ModelMaxZ - 55.0f, 0.0f, ModelMaxZ, ShipMinZ, ShipMaxZ);

  SceneComposer scene(*this, renderer, sound, camera);

  const MeshHandle skyboxMesh = scene.LoadMesh(
      RenderType::Skybox,
      {.model = SKYBOX_MODEL_FILENAME,
       .texture = SKYBOX_TEXTURE_FILENAME,
       .meshTransform = glm::scale(glm::mat4(1.0f), glm::vec3(2000.0f))});
  const MeshHandle sunMesh = scene.LoadMesh(
      RenderType::Sun,
      {.model = SPHERE_MODEL_FILENAME, .texture = SUN_TEXTURE_FILENAME});
  const MeshHandle playerMesh = scene.LoadMesh(
      RenderType::Spaceship, {.model = SPACESHIP_STATIONARY_MODEL_FILENAME,
                              .texture = SPACESHIP_TEXTURE_FILENAME,
                              .emissive = SPACESHIP_EMISSIVE_FILENAME});
  const MeshHandle npcMesh = scene.LoadMesh(
      RenderType::Spaceship, {.model = NPC_SPACESHIP_MODEL_FILENAME,
                              .texture = NPC_SPACESHIP_TEXTURE_FILENAME,
                              .emissive = NPC_SPACESHIP_EMISSIVE_FILENAME});
  const MeshHandle exhaustParticleMesh = scene.LoadMesh(
      RenderType::Particle,
      {.model = PARTICLE_MODEL_FILENAME,
       .meshTransform = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f))});

  const CompositionBuilder spaceshipExhaust = std::move(
      scene.ParticleSystem(ParticleBehaviour::SpaceshipExhaust)
          .Attach(BEHAVIOUR(ExhaustParticleController,
                            parent->RetrieveProperty<Transform>(),
                            actor.RetrieveProperty<ParticleController>()))
          .Mesh(exhaustParticleMesh)
          .SpawnRegion(glm::vec3(x1, y1, z1), glm::vec3(x2, y2, z2)));

  scene.Actor()
      .Attach(
          BEHAVIOUR(BackgroundMusic, actor.RetrieveProperty<SoundEmitter>()))
      .Spawn();
  scene.Actor().Mesh(skyboxMesh).Spawn();
  scene.Actor()
      .Attach(BEHAVIOUR(SunMovement, actor.RetrieveProperty<Transform>()))
      .Mesh(sunMesh)
      .Child(scene.Actor().Mesh(npcMesh))
      .Spawn();
  scene.Actor()
      .Attach(BEHAVIOUR(ForwardMovement, actor.RetrieveProperty<Transform>()))
      .Mesh(npcMesh)
      .Child(spaceshipExhaust)
      .Spawn();
  scene.Actor()
      .Attach(BEHAVIOUR(PlayerMovement, actor.RetrieveProperty<Transform>()))
      .Mesh(playerMesh)
      .Child(scene.Camera())
      .Child(spaceshipExhaust)
      .Spawn();
}

void Scene::Consume(std::unique_ptr<game::Actor> value) {
  actors_.push_back(std::move(value));
}

void Scene::UpdateModel(const UpdateContext& context) {
  for (const auto& actor : actors_) {
    actor->UpdateModel(context);
  }
}
