#ifndef VULKAN_SRC_SYSTEM_WINDOWING_WINDOWEVENTCONSUMER_H_
#define VULKAN_SRC_SYSTEM_WINDOWING_WINDOWEVENTCONSUMER_H_

#include "general/operations/Consumer.h"
#include "util/include/sdl.h"

using WindowEventConsumer = Consumer<const SDL_Event&>;

#endif  // VULKAN_SRC_SYSTEM_WINDOWING_WINDOWEVENTCONSUMER_H_
