#ifndef VULKAN_SRC_GAME_RENDERS_PARTICLES_PARTICLESPAWNER_H
#define VULKAN_SRC_GAME_RENDERS_PARTICLES_PARTICLESPAWNER_H

#include "ParticleInstance.h"
#include "game/renders/ActorGenerator.h"
#include "game/renders/ActorSpawnController.h"
#include "game/renders/ParticleController.h"
#include "game/renders/SceneRender.h"

class ParticleSpawner : public ActorGenerator,
                        public SceneRender,
                        public ParticleController {
 public:
  std::unique_ptr<PipelineStateFactory> ConfigurePipeline() const override;
  std::unique_ptr<DescriptorConfiguration> ConfigureDescriptors()
      const override;
  void LoadActors(ResourceLoader& resourceLoader,
                  ActorSpawnController& actorSpawnController) override;

  ParticleStream& CreateParticleStream() override;

  std::unique_ptr<Actor> CreateActor(
      ResourceLoader& resourceLoader,
      ResourceBinder& resourceBinder) const override;

 private:
  ActorSpawnController* actorSpawnController_;
  MultiParticleMesh mesh_;
};

#endif  // VULKAN_SRC_GAME_RENDERS_PARTICLES_PARTICLESPAWNER_H
