#ifndef VULKAN_SRC_GAME_RENDERS_PARTICLESTREAM_H
#define VULKAN_SRC_GAME_RENDERS_PARTICLESTREAM_H

#include "general/geometry/Rect.h"
#include "util/include/glm.h"

class ParticleStream {
 public:
  virtual ~ParticleStream() = default;

  virtual void SetSpawnArea(const Rectf& spawnArea) = 0;
  virtual void SetTransform(const glm::mat4& transform) = 0;
  virtual void SetEnabled(const bool enabled) = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERS_PARTICLESTREAM_H
