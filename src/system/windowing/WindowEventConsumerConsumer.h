#ifndef VULKAN_SRC_SYSTEM_WINDOWING_WINDOWEVENTCONSUMERCONSUMER_H_
#define VULKAN_SRC_SYSTEM_WINDOWING_WINDOWEVENTCONSUMERCONSUMER_H_

#include "WindowEventConsumer.h"

class WindowEventConsumerConsumer {
 public:
  virtual ~WindowEventConsumerConsumer() = default;

  virtual void Consume(WindowEventConsumer& consumer) = 0;
};

#endif  // VULKAN_SRC_SYSTEM_WINDOWING_WINDOWEVENTCONSUMERCONSUMER_H_
