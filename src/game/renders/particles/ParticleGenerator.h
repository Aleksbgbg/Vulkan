#ifndef VULKAN_SRC_GAME_RENDERS_PARTICLES_PARTICLEGENERATOR_H
#define VULKAN_SRC_GAME_RENDERS_PARTICLES_PARTICLEGENERATOR_H

#include <array>

#include "MultiParticleMesh.h"
#include "game/Actor.h"
#include "game/rendering/resources/ResourceLoader.h"
#include "game/renders/ParticleController.h"
#include "general/algorithms/RandomNumberGenerator.h"

inline constexpr u32 InstanceCount = 1024;

class ParticleGenerator : public Actor, public ParticleController {
 public:
  ParticleGenerator() = default;
  ParticleGenerator(const ResourceLoader& resourceLoader);

  const Mesh& GetMesh() const override;
  void BindBuffers(TextureRegistry& resourceBinder) const override;

  void SetSpawnArea(const Rectf& spawnArea) override;
  void SetTransform(const glm::mat4& transform) override;
  void SetEnabled(const bool enabled) override;

  void UpdateModel(const UpdateContext& context) override;
  void Render(const MeshRenderer& renderer) const override;

 private:
  void KillAllParticles();

 private:
  struct ParticleRender {
    alignas(16) glm::mat4 model;
    alignas(4) float fractionOfLife;
  };

  struct ParticleRenderData {
    ParticleRender particleRender[InstanceCount];
  };

  struct ParticleInfo {
    float lifespan;
    float timeToLive;
    glm::vec3 velocity;
    glm::vec3 position;
    glm::vec3 rotation;
  };

  RandomNumberGenerator randomNumberGenerator;
  MultiParticleMesh mesh;
  BufferWithMemory instanceParameters;
  std::array<ParticleInfo, InstanceCount> particles;
  glm::mat4 transform;
  Rectf spawnArea;
  bool enabled;
};

#endif  // VULKAN_SRC_GAME_RENDERS_PARTICLES_PARTICLEGENERATOR_H
