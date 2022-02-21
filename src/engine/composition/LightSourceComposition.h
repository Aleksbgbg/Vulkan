#ifndef VULKAN_SRC_ENGINE_COMPOSITION_LIGHTSOURCECOMPOSITION_H_
#define VULKAN_SRC_ENGINE_COMPOSITION_LIGHTSOURCECOMPOSITION_H_

#include "LightSource.h"

class LightSourceComposition {
 public:
  virtual ~LightSourceComposition() = default;

  virtual LightSource GetLightSource() const = 0;
};

#endif  // VULKAN_SRC_ENGINE_COMPOSITION_LIGHTSOURCECOMPOSITION_H_
