#include "UpAndDownGlidingAnimation.h"

UpAndDownGlidingAnimation::UpAndDownGlidingAnimation(float bot, float top,
                                                     float step)
    : bot(bot), top(top), step(step), direction(1.0f), value(bot) {}

float UpAndDownGlidingAnimation::UpdateValue(const float deltaTime) {
  value += step * deltaTime * direction;

  if (value > top) {
    const float difference = value - top;
    value -= difference * 2.0f;

    direction = -1.0f;
  }

  if (value < bot) {
    const float difference = bot - value;
    value += difference * 2.0f;

    direction = 1.0f;
  }

  return value;
}
