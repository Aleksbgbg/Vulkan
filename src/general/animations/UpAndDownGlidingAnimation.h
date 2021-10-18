#ifndef VULKAN_SRC_UPANDDOWNGLIDINGANIMATION_H
#define VULKAN_SRC_UPANDDOWNGLIDINGANIMATION_H

class UpAndDownGlidingAnimation {
 public:
  UpAndDownGlidingAnimation(float bot, float top, float step);

  float UpdateValue(const float deltaTime);

 private:
  float bot;
  float top;
  float step;

  float direction;

  float value;
};

#endif  // VULKAN_SRC_UPANDDOWNGLIDINGANIMATION_H
