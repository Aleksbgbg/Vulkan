#ifndef VULKAN_SRC_GAME_UI_BINDABLE_H_
#define VULKAN_SRC_GAME_UI_BINDABLE_H_

#include <memory>

#include "Bound.h"
#include "ReadWriteable.h"
#include "UnboundValue.h"

template <typename T>
class Bindable {
 public:
  explicit Bindable(T value)
      : deleter_(std::make_unique<UnboundValue<T>>(value)),
        readWriteable_(*deleter_) {}
  explicit Bindable(Bound<T>& boundValue)
      : deleter_(), readWriteable_(boundValue) {}

  operator T() const {
    return readWriteable_.GetValue();
  }
  const Bindable& operator=(const T value) const {
    readWriteable_.SetValue(value);
    return *this;
  }

 private:
  std::unique_ptr<ReadWriteable<T>> deleter_;
  ReadWriteable<T>& readWriteable_;
};

#endif  // VULKAN_SRC_GAME_UI_BINDABLE_H_
