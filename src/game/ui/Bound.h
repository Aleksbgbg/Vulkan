#ifndef VULKAN_SRC_GAME_UI_BOUND_H_
#define VULKAN_SRC_GAME_UI_BOUND_H_

#include <functional>

#include "Binding.h"
#include "ReadWriteable.h"

template <typename T>
class Bound : public ui::Binding, public ReadWriteable<T> {
 public:
  explicit Bound(std::function<void(T)> onSet) : onSet_(onSet), value_() {}

  T GetValue() const {
    return value_;
  }

  void SetValue(const T value) {
    if (value_ != value) {
      value_ = value;
      onSet_(value);
    }
  }

 private:
  std::function<void(T)> onSet_;
  T value_;
};

#endif  // VULKAN_SRC_GAME_UI_BOUND_H_
