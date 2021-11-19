#include "ParticleInstance.h"

ParticleInstance::ParticleInstance(const MultiParticleMesh& mesh,
                                   ResourceLoader& resourceLoader,
                                   BufferBinder& bufferBinder)
    : randomNumberGenerator(),
      mesh(mesh),
      instanceParameters(resourceLoader.AllocateLocalBuffer(
          sizeof(ParticleRenderData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)),
      particles(),
      transform(1.0f) {
  bufferBinder.BindBuffer(instanceParameters.buffer, VK_WHOLE_SIZE,
                          VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0);

  KillAllParticles();
}

void ParticleInstance::SetSpawnArea(const Rectf& spawnArea) {
  this->spawnArea = spawnArea;
}

void ParticleInstance::SetTransform(const glm::mat4& transform) {
  this->transform = transform;
}

void ParticleInstance::SetEnabled(const bool enabled) {
  this->enabled = enabled;
}

float RandomRotation(RandomNumberGenerator& randomNumberGenerator) {
  return randomNumberGenerator.RandomFloat(-2.0f * glm::pi<float>(),
                                           2.0f * glm::pi<float>());
}

void ParticleInstance::UpdateModel(const UpdateContext& context) {
  void* const instanceParametersMemory =
      instanceParameters.memory.Map(0, sizeof(ParticleRenderData));
  ParticleRenderData& particleRenderData =
      *reinterpret_cast<ParticleRenderData* const>(instanceParametersMemory);

  for (u32 index = 0; index < InstanceCount; ++index) {
    ParticleInfo& particle = particles[index];

    particle.timeToLive -= context.deltaTime;

    if (particle.timeToLive < 0.0f) {
      if (enabled) {
        particle.lifespan = randomNumberGenerator.RandomFloat(0.0f, 1.0f);
        particle.timeToLive = particle.lifespan;
        particle.baseTransform = transform;
        particle.velocity.z = -randomNumberGenerator.RandomFloat(0.0f, 0.5f);
        particle.position = glm::vec3(
            randomNumberGenerator.RandomFloat(spawnArea.X1(), spawnArea.X2()),
            randomNumberGenerator.RandomFloat(spawnArea.Y1(), spawnArea.Y2()),
            0.0f);
        particle.rotation = glm::vec3(RandomRotation(randomNumberGenerator),
                                      RandomRotation(randomNumberGenerator),
                                      RandomRotation(randomNumberGenerator));
      } else {
        particle.position = glm::vec3(-99999.0f);
      }
    } else {
      particle.position.z += particle.velocity.z;
    }

    ParticleRender& particleRender = particleRenderData.particleRender[index];

    particleRender.model =
        glm::translate(particle.baseTransform, particle.position) *
        glm::toMat4(glm::quat(particle.rotation));
    particleRender.fractionOfLife = particle.timeToLive / particle.lifespan;
  }

  instanceParameters.memory.Unmap();
}

const Mesh& ParticleInstance::GetMesh() const {
  return mesh;
}

void ParticleInstance::Render(const MeshRenderer& renderer) const {
  renderer.Render(mesh);
}

void ParticleInstance::KillAllParticles() {
  for (u32 index = 0; index < particles.size(); ++index) {
    particles[index].timeToLive = -1.0f;
  }
}
