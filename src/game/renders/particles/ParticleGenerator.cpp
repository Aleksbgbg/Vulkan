#include "ParticleGenerator.h"

#include "game/rendering/vertices/PositionVertex.h"

MultiParticleMesh LoadMesh(const ResourceLoader& resourceLoader) {
  constexpr float scale = 0.05f;
  const std::vector<PositionVertex> vertices = {
      /* 0 */ {{-scale, -scale, -scale}},
      /* 1 */ {{-scale, scale, -scale}},
      /* 2 */ {{scale, scale, -scale}},
      /* 3 */ {{scale, -scale, -scale}},
      /* 4 */ {{0.0f, 0.0f, scale}},
  };
  const std::vector<u16> indices = {
      0, 1, 2, 2, 3, 0,  // bottom face
      0, 4, 1,           // back face
      1, 4, 2,           // left face
      2, 4, 3,           // right face
      3, 4, 0,           // front face
  };

  return MultiParticleMesh(resourceLoader.AllocateVertexBuffer(vertices),
                           resourceLoader.AllocateIndexBuffer(indices),
                           indices.size(), InstanceCount);
}

ParticleGenerator::ParticleGenerator(const ResourceLoader& resourceLoader)
    : randomNumberGenerator(),
      mesh(LoadMesh(resourceLoader)),
      instanceParameters(resourceLoader.AllocateLocalBuffer(
          sizeof(ParticleRenderData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)),
      particles(),
      transform(1.0f) {
  KillAllParticles();
}

const Mesh& ParticleGenerator::GetMesh() const {
  return mesh;
}

void ParticleGenerator::BindBuffers(TextureRegistry& resourceBinder) const {
  resourceBinder.BindBuffer(instanceParameters.buffer, VK_WHOLE_SIZE,
                            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0);
}

void ParticleGenerator::SetSpawnArea(const Rectf& spawnArea) {
  this->spawnArea = spawnArea;
}

void ParticleGenerator::SetTransform(const glm::mat4& transform) {
  this->transform = transform;
}

void ParticleGenerator::SetEnabled(const bool enabled) {
  this->enabled = enabled;
}

float RandomRotation(RandomNumberGenerator& randomNumberGenerator) {
  return randomNumberGenerator.RandomFloat(-2.0f * glm::pi<float>(),
                                           2.0f * glm::pi<float>());
}

void ParticleGenerator::UpdateModel(const UpdateContext& context) {
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

void ParticleGenerator::Render(const MeshRenderer& renderer) const {
  renderer.Render(mesh);
}

void ParticleGenerator::KillAllParticles() {
  for (u32 index = 0; index < particles.size(); ++index) {
    particles[index].timeToLive = -1.0f;
  }
}
