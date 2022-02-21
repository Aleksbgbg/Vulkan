#ifndef VULKAN_SRC_ENGINE_UI_READWRITEABLE_H_
#define VULKAN_SRC_ENGINE_UI_READWRITEABLE_H_

template <typename T>
class ReadWriteable {
 public:
  virtual ~ReadWriteable() = default;

  virtual T GetValue() const = 0;
  virtual void SetValue(const T value) = 0;
};

#endif  // VULKAN_SRC_ENGINE_UI_READWRITEABLE_H_
