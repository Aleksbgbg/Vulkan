#ifndef VULKAN_SRC_ENGINE_UI_COLOR_H_
#define VULKAN_SRC_ENGINE_UI_COLOR_H_

struct Color {
  float r;
  float g;
  float b;
  float a;
};

inline constexpr Color WHITE = {1.0f, 1.0f, 1.0f, 1.0f};
inline constexpr Color BLACK = {0.0f, 0.0f, 0.0f, 1.0f};

#endif  // VULKAN_SRC_ENGINE_UI_COLOR_H_
