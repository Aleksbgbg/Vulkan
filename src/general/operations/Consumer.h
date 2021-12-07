#ifndef VULKAN_SRC_GENERAL_OPERATIONS_CONSUMER_H_
#define VULKAN_SRC_GENERAL_OPERATIONS_CONSUMER_H_

template <typename T>
class Consumer {
 public:
  virtual ~Consumer() = default;

  virtual void Consume(T value) = 0;
};

#endif  // VULKAN_SRC_GENERAL_OPERATIONS_CONSUMER_H_
