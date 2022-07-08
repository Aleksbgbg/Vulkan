#ifndef SRC_RENDERER_VULKAN_PARTICLECOMPUTEHANDLER_H_
#define SRC_RENDERER_VULKAN_PARTICLECOMPUTEHANDLER_H_

#include "core/algorithms/RandomNumberGenerator.h"
#include "engine/property/traits/Transformable.h"
#include "engine/property/traits/Visible.h"
#include "renderer/vulkan/render_graph/HostDescriptorWriter.h"

class ParticleComputeHandler : public HostDescriptorWriter {
 public:
  ParticleComputeHandler(RandomNumberGenerator& randomNumberGenerator,
                         const Transformable& transformable,
                         const Visible& visible, glm::vec3 spawnRegionLow,
                         glm::vec3 spawnRegionHigh);

  void Fill(
      const std::unordered_map<u32, Descriptor>& descriptors) const override;

 private:
  RandomNumberGenerator& randomNumberGenerator_;
  const Transformable& transformable_;
  const Visible& visible_;
  glm::vec3 spawnRegionLow_;
  glm::vec3 spawnRegionHigh_;
};

#endif  // SRC_RENDERER_VULKAN_PARTICLECOMPUTEHANDLER_H_
