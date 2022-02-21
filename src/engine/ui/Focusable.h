#ifndef VULKAN_SRC_ENGINE_UI_FOCUSABLE_H_
#define VULKAN_SRC_ENGINE_UI_FOCUSABLE_H_

class Focusable {
 public:
  virtual ~Focusable() = default;

  virtual void OnAcquiredFocus() = 0;
  virtual void OnChildAcquiredFocus() = 0;
  virtual void OnLostFocus() = 0;
};

#endif  // VULKAN_SRC_ENGINE_UI_FOCUSABLE_H_
