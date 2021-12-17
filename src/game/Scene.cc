#include "Scene.h"

#include "game/behaviours/BackgroundMusic.h"
#include "game/behaviours/ConstantMovement.h"
#include "game/behaviours/ExhaustParticleController.h"
#include "game/behaviours/LaserEmitter.h"
#include "game/behaviours/PlayerMovement.h"
#include "game/behaviours/SunMovement.h"
#include "game/composition/ParticleBehaviour.h"
#include "game/composition/SceneComposer.h"
#include "general/math/math.h"
#include "util/filenames.h"

Scene::Scene(Renderer& renderer, sys::Sound& sound, game::Camera& camera)
    : scene_(camera, {.actorConsumer = this,
                      .actorOwner = this,
                      .renderer = &renderer,
                      .sound = &sound}),
      sceneGraph_(),
      actorsToSpawn_(),
      actorsToDespawn_() {
  constexpr float ShipMinX = -1.7f;
  constexpr float ShipMaxX = 1.7f;
  constexpr float ShipMinY = -0.8f;
  constexpr float ShipMaxY = 0.8f;
  constexpr float ShipMinZ = -2.4f;
  constexpr float ShipMaxZ = 2.8f;

  constexpr float ModelMaxX = 34.0f;
  constexpr float ModelMaxY = 16.0f;
  constexpr float ModelMaxZ = 56.0f;

  // Ship exhaust spawn position
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

  const MeshHandle skyboxMesh = scene_.LoadMesh(
      RenderType::Skybox,
      {.model = SKYBOX_MODEL_FILENAME,
       .texture = SKYBOX_TEXTURE_FILENAME,
       .meshTransform = glm::scale(glm::mat4(1.0f), glm::vec3(2000.0f))});
  const MeshHandle sunMesh = scene_.LoadMesh(
      RenderType::Sun,
      {.model = SPHERE_MODEL_FILENAME, .texture = SUN_TEXTURE_FILENAME});
  const MeshHandle playerMesh = scene_.LoadMesh(
      RenderType::Spaceship, {.model = SPACESHIP_STATIONARY_MODEL_FILENAME,
                              .texture = SPACESHIP_TEXTURE_FILENAME,
                              .emissive = SPACESHIP_EMISSIVE_FILENAME});
  const MeshHandle npcMesh = scene_.LoadMesh(
      RenderType::Spaceship, {.model = NPC_SPACESHIP_MODEL_FILENAME,
                              .texture = NPC_SPACESHIP_TEXTURE_FILENAME,
                              .emissive = NPC_SPACESHIP_EMISSIVE_FILENAME});
  const MeshHandle exhaustParticleMesh = scene_.LoadMesh(
      RenderType::Particle,
      {.model = PARTICLE_MODEL_FILENAME,
       .meshTransform = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f))});
  const MeshHandle laserMesh = scene_.LoadMesh(
      RenderType::Sun,
      {.model = LASER_MODEL_FILENAME, .texture = LASER_TEXTURE_FILENAME});

  const CompositionBuilder spaceshipExhaust = std::move(
      scene_.ParticleSystem(ParticleBehaviour::SpaceshipExhaust)
          .Attach(BEHAVIOUR(ExhaustParticleController,
                            parent->RetrieveProperty<Transform>(),
                            actor.RetrieveProperty<ParticleController>()))
          .Mesh(exhaustParticleMesh)
          .SpawnRegion(glm::vec3(x1, y1, z1), glm::vec3(x2, y2, z2)));

  scene_.Actor()
      .Attach(
          BEHAVIOUR(BackgroundMusic, actor.RetrieveProperty<SoundEmitter>()))
      .Spawn();
  scene_.Actor().Mesh(skyboxMesh).Spawn();
  scene_.Actor()
      .Attach(BEHAVIOUR(SunMovement, actor.RetrieveProperty<Transform>()))
      .Mesh(sunMesh)
      .Child(scene_.Actor().Mesh(npcMesh))
      .Spawn();
  scene_.Actor()
      .Attach(BEHAVIOUR(ConstantMovement, actor.RetrieveProperty<Transform>(),
                        glm::vec3(0.0f, 0.0f, 1.0f)))
      .Mesh(npcMesh)
      .Child(spaceshipExhaust)
      .Spawn();
  scene_.Actor()
      .Attach(BEHAVIOUR(PlayerMovement, actor.RetrieveProperty<Transform>()))
      .Attach(BEHAVIOUR(LaserEmitter, actor.RetrieveProperty<Transform>(),
                        scene_, laserMesh))
      .Mesh(playerMesh)
      .Child(scene_.Camera())
      .Child(spaceshipExhaust)
      .Spawn();
}

void Scene::Consume(ActorToSpawn value) {
  actorsToSpawn_.push_back(std::move(value));
}

void Scene::DespawnActor(const ActorKey key) {
  actorsToDespawn_.push_back(key);
}

void Scene::Update(const UpdateContext& context) {
  for (const ActorKey key : actorsToDespawn_) {
    sceneGraph_.Remove(key);
  }
  actorsToDespawn_.clear();

  for (auto& spawn : actorsToSpawn_) {
    sceneGraph_.Add(spawn.parent, spawn.key, std::move(spawn.actor));
  }
  actorsToSpawn_.clear();

  for (const auto& pair : sceneGraph_) {
    pair.second->UpdateModel(context);
  }
}
