#ifndef VULKAN_SRC_RENDERER_VULKAN_PARTICLECOMPUTEHANDLER_H_
#define VULKAN_SRC_RENDERER_VULKAN_PARTICLECOMPUTEHANDLER_H_

#include "game/SpawnControllable.h"
#include "game/Transformable.h"
#include "general/algorithms/RandomNumberGenerator.h"
#include "renderer/vulkan/render_graph/HostDescriptorWriter.h"

class ParticleComputeHandler : public HostDescriptorWriter {
 public:
  ParticleComputeHandler(RandomNumberGenerator& randomNumberGenerator,
                         const SpawnControllable& spawnControllable,
                         const Transformable& transformable,
                         glm::vec3 spawnRegionLow, glm::vec3 spawnRegionHigh);

  void Fill(
      const std::unordered_map<u32, Descriptor>& descriptors) const override;

 private:
  RandomNumberGenerator& randomNumberGenerator_;
  const SpawnControllable& spawnControllable_;
  const Transformable& transformable_;
  glm::vec3 spawnRegionLow_;
  glm::vec3 spawnRegionHigh_;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_PARTICLECOMPUTEHANDLER_H_
