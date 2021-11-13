#ifndef VULKAN_SRC_GAME_RENDERS_PARTICLES_PARTICLERENDER_H
#define VULKAN_SRC_GAME_RENDERS_PARTICLES_PARTICLERENDER_H

#include "ParticleGenerator.h"
#include "game/renders/ParticleController.h"
#include "game/renders/SceneRender.h"

class ParticleRender : public SceneRender {
 public:
  ParticleRender();

  std::unique_ptr<PipelineStateFactory> ConfigurePipeline() const override;
  std::unique_ptr<DescriptorConfiguration> ConfigureDescriptors()
      const override;
  std::vector<std::unique_ptr<Actor>> LoadActors(
      const ResourceLoader& resourceLoader) override;

  ParticleController& GetParticleController() const;

 private:
  std::unique_ptr<ParticleGenerator> particleGenerator;
};

#endif  // VULKAN_SRC_GAME_RENDERS_PARTICLES_PARTICLERENDER_H
