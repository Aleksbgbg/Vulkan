#ifndef VULKAN_SRC_GAME_VISIBLE_H_
#define VULKAN_SRC_GAME_VISIBLE_H_

class Visible {
 public:
  virtual ~Visible() = default;

  virtual bool IsVisible() const = 0;
};

#endif  // VULKAN_SRC_GAME_VISIBLE_H_
