#include "NormalizedOneTimeFunctionAnimation.h"

NormalizedOneTimeFunctionAnimation::NormalizedOneTimeFunctionAnimation(
    std::function<float(float)> function, float endTime)
    : function(function),
      endTime(endTime),
      maxValue(function(endTime)),
      currentTime(0.0f) {}

bool NormalizedOneTimeFunctionAnimation::Finished() {
  return currentTime >= endTime;
}

float NormalizedOneTimeFunctionAnimation::UpdateValue(const float deltaTime) {
  currentTime += deltaTime;
  return function(std::min(currentTime, endTime)) / maxValue;
}
