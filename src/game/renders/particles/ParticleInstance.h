#ifndef VULKAN_SRC_GAME_RENDERS_PARTICLES_PARTICLEINSTANCE_H_
#define VULKAN_SRC_GAME_RENDERS_PARTICLES_PARTICLEINSTANCE_H_

#include <array>

#include "MultiParticleMesh.h"
#include "game/Actor.h"
#include "game/rendering/resources/BufferBinder.h"
#include "game/rendering/resources/ResourceLoader.h"
#include "game/renders/ParticleStream.h"
#include "general/algorithms/RandomNumberGenerator.h"

inline constexpr u32 InstanceCount = 1024;

class ParticleInstance : public Actor, public ParticleStream {
 public:
  ParticleInstance(const MultiParticleMesh& mesh,
                   ResourceLoader& resourceLoader, BufferBinder& bufferBinder);

  void SetSpawnArea(const Rectf& spawnArea) override;
  void SetTransform(const glm::mat4& transform) override;
  void SetEnabled(const bool enabled) override;

  void UpdateModel(const UpdateContext& context) override;
  const Mesh& GetMesh() const override;
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
    glm::mat4 baseTransform;
    glm::vec3 velocity;
    glm::vec3 position;
    glm::vec3 rotation;
  };

  RandomNumberGenerator randomNumberGenerator_;
  const MultiParticleMesh& mesh_;
  BufferWithMemory instanceParameters_;
  std::array<ParticleInfo, InstanceCount> particles_;
  glm::mat4 transform_;
  Rectf spawnArea_;
  bool enabled_;
  u32 aliveParticles_;
};

#endif  // VULKAN_SRC_GAME_RENDERS_PARTICLES_PARTICLEINSTANCE_H_
