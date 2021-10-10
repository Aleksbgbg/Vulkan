#ifndef VULKAN_SRC_NORMALIZEDONETIMEFUNCTIONANIMATION_H
#define VULKAN_SRC_NORMALIZEDONETIMEFUNCTIONANIMATION_H

#include <functional>

class NormalizedOneTimeFunctionAnimation {
 public:
  NormalizedOneTimeFunctionAnimation(std::function<float(float)> function, float endTime);

  bool Finished();
  float UpdateValue(const float deltaTime);

 private:
  std::function<float(float)> function;
  float endTime;

  float maxValue;

  float currentTime;
};

#endif  // VULKAN_SRC_NORMALIZEDONETIMEFUNCTIONANIMATION_H
