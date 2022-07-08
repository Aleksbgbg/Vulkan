#ifndef SRC_ENGINE_COMPOSITION_LIGHTSOURCECOMPOSITION_H_
#define SRC_ENGINE_COMPOSITION_LIGHTSOURCECOMPOSITION_H_

#include "LightSource.h"

class LightSourceComposition {
 public:
  virtual ~LightSourceComposition() = default;

  virtual LightSource GetLightSource() const = 0;
};

#endif  // SRC_ENGINE_COMPOSITION_LIGHTSOURCECOMPOSITION_H_
