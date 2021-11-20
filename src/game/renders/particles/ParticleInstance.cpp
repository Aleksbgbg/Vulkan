#include "ParticleInstance.h"

ParticleInstance::ParticleInstance(const MultiParticleMesh& mesh,
                                   ResourceLoader& resourceLoader,
                                   BufferBinder& bufferBinder)
    : randomNumberGenerator_(),
      mesh_(mesh),
      instanceParameters_(resourceLoader.AllocateLocalBuffer(
          sizeof(ParticleRenderData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)),
      particles_(),
      transform_(1.0f),
      aliveParticles_(0) {
  bufferBinder.BindBuffer(instanceParameters_.buffer, VK_WHOLE_SIZE,
                          VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0);

  KillAllParticles();
}

void ParticleInstance::SetSpawnArea(const Rectf& spawnArea) {
  spawnArea_ = spawnArea;
}

void ParticleInstance::SetTransform(const glm::mat4& transform) {
  transform_ = transform;
}

void ParticleInstance::SetEnabled(const bool enabled) {
  enabled_ = enabled;
}

float RandomRotation(RandomNumberGenerator& randomNumberGenerator) {
  return randomNumberGenerator.RandomFloat(-FullTurn, FullTurn);
}

void ParticleInstance::UpdateModel(const UpdateContext& context) {
  void* const instanceParametersMemory =
      instanceParameters_.memory.Map(0, sizeof(ParticleRenderData));
  ParticleRenderData& particleRenderData =
      *reinterpret_cast<ParticleRenderData* const>(instanceParametersMemory);

  for (u32 index = 0; index < aliveParticles_; ++index) {
    ParticleInfo& particle = particles_[index];

    particle.timeToLive -= context.deltaTime;
    particle.position.z += particle.velocity.z;
  }

  for (u32 index = 0; index < aliveParticles_;) {
    ParticleInfo& particle = particles_[index];

    if (particle.timeToLive < 0.0f) {
      --aliveParticles_;
      std::swap(particle, particles_[aliveParticles_]);
    } else {
      ++index;
    }
  }

  if (enabled_) {
    for (u32 index = aliveParticles_; index < particles_.size(); ++index) {
      ParticleInfo& particle = particles_[index];
      particle.lifespan = randomNumberGenerator_.RandomFloat(0.0f, 1.0f);
      particle.timeToLive = particle.lifespan;
      particle.baseTransform = transform_;
      particle.velocity.z = -randomNumberGenerator_.RandomFloat(0.0f, 0.5f);
      particle.position = glm::vec3(
          randomNumberGenerator_.RandomFloat(spawnArea_.X1(), spawnArea_.X2()),
          randomNumberGenerator_.RandomFloat(spawnArea_.Y1(), spawnArea_.Y2()),
          0.0f);
      particle.rotation = glm::vec3(RandomRotation(randomNumberGenerator_),
                                    RandomRotation(randomNumberGenerator_),
                                    RandomRotation(randomNumberGenerator_));
    }

    aliveParticles_ = particles_.size();
  }

  for (u32 index = 0; index < aliveParticles_; ++index) {
    ParticleInfo& particle = particles_[index];
    ParticleRender& particleRender = particleRenderData.particleRender[index];

    particleRender.model =
        glm::translate(particle.baseTransform, particle.position) *
        glm::toMat4(glm::quat(particle.rotation));
    particleRender.fractionOfLife = particle.timeToLive / particle.lifespan;
  }

  instanceParameters_.memory.Unmap();
}

const Mesh& ParticleInstance::GetMesh() const {
  return mesh_.RenderInstances(aliveParticles_);
}

void ParticleInstance::Render(const MeshRenderer& renderer) const {
  renderer.Render(mesh_.RenderInstances(aliveParticles_));
}

void ParticleInstance::KillAllParticles() {
  for (u32 index = 0; index < particles_.size(); ++index) {
    particles_[index].timeToLive = -1.0f;
  }
}
