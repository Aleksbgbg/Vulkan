#include "Scene.h"

#include "engine/composition/ParticleBehaviour.h"
#include "engine/composition/PointLight.h"
#include "engine/composition/SceneComposer.h"
#include "engine/composition/behaviour_utils.h"
#include "game/behaviours/BackgroundMusic.h"
#include "game/behaviours/ConstantMovement.h"
#include "game/behaviours/DespawnAfterPeriod.h"
#include "game/behaviours/ExhaustParticleController.h"
#include "game/behaviours/LaserEmitter.h"
#include "game/behaviours/PauseMenuToggle.h"
#include "game/behaviours/PlayerMovement.h"
#include "game/behaviours/SunMovement.h"
#include "game/viewmodels/PauseMenuViewModel.h"
#include "ui_pages.h"
#include "util/filenames.h"

Scene::Scene(Renderer& renderer, sys::Window& window, sys::Sound& sound,
             game::Camera& camera, const FontAtlas& fontAtlas,
             Settings& settings,
             GraphicsSettingsConfigurator& graphicsSettingsConfigurator)
    : scene_(camera, *this, *this, renderer, window, sound, fontAtlas),
      sceneGraph_(),
      actorsToSpawn_(),
      actorsToDespawn_() {
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

  const SoundHandle backgroundMusic =
      sound.LoadSound(BACKGROUND_MUSIC_FILENAME);
  const SoundHandle laserSoundEffect = sound.LoadSound(LASER_SOUND_FILENAME);

  const CompositionBuilder spaceshipExhaust =
      std::move(scene_.ParticleSystem(ParticleBehaviour::SpaceshipExhaust)
                    .Attach(BEHAVIOUR(ExhaustParticleController,
                                      parent->RetrieveProperty<Transform>(),
                                      actor.RetrieveProperty<Visibility>()))
                    .Mesh(exhaustParticleMesh)
                    .SpawnRegion(glm::vec3(-1.1f, -0.3f, -2.4f),
                                 glm::vec3(1.0f, -0.1f, -2.31f)));

  scene_.Actor()
      .Attach(BEHAVIOUR(BackgroundMusic, actor.RetrieveProperty<SoundEmitter>(),
                        backgroundMusic))
      .Spawn();
  scene_.Actor().Mesh(skyboxMesh).Spawn();
  scene_.Actor()
      .LightSource(PointLight()
                       .Color(glm::vec3(77.0f / 255.0f, 77.0f / 255.0f, 1.0f))
                       .AmbientFactor(0.02f)
                       .AttenuationLinear(0.0014f)
                       .AttenuationQuadratic(0.0f))
      .Mesh(sunMesh)
      .Attach(BEHAVIOUR(SunMovement, actor.RetrieveProperty<Transform>()))
      .Spawn();
  scene_.Actor()
      .Attach(BEHAVIOUR(ConstantMovement,
                        ConstantMovement::ParameterPack()
                            .SetTransform(actor.RetrieveProperty<Transform>())
                            .SetForwardVelocity(1.0f)))
      .Mesh(npcMesh)
      .Child(spaceshipExhaust)
      .Spawn();
  scene_.Actor()
      .Attach(BEHAVIOUR(PlayerMovement, actor.RetrieveProperty<Transform>()))
      .Attach(BEHAVIOUR(
          LaserEmitter,
          LaserEmitter::ParameterPack()
              .SetParentTransform(actor.RetrieveProperty<Transform>())
              .SetLaserComposition(
                  scene_.Actor()
                      .Attach(BEHAVIOUR(DespawnAfterPeriod, actor, 5.0f))
                      .LightSource(PointLight()
                                       .Color(glm::vec3(1.0f, 0.0f, 0.0f))
                                       .AttenuationLinear(0.22f)
                                       .AttenuationQuadratic(0.20f))
                      .Mesh(laserMesh))
              .SetSoundEmitter(actor.RetrieveProperty<SoundEmitter>())
              .SetSoundEffect(laserSoundEffect)))
      .Mesh(playerMesh)
      .Child(scene_.Camera())
      .Child(spaceshipExhaust)
      .Spawn();

  scene_
      .UiElement(PAUSE_MENU, std::make_unique<PauseMenuViewModel>(
                                 settings, graphicsSettingsConfigurator))
      .Attach(BEHAVIOUR(PauseMenuToggle,
                        actor.RetrieveProperty<GraphicalInterface>()))
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

  for (const auto& actor : sceneGraph_) {
    actor->UpdateModel(context);
  }
}
