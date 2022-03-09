#ifndef VULKAN_SRC_CORE_PROFILING_PROFILE_H_
#define VULKAN_SRC_CORE_PROFILING_PROFILE_H_

#include <chrono>
#include <iostream>

#include "util/build_definition.h"

#if defined(PROFILING)

#define PROFILING_BEGIN(MESSAGE)  \
  const auto __MESSAGE = MESSAGE; \
  const auto __START_TIME = std::chrono::high_resolution_clock::now();

#define PROFILING_END                                                  \
  const auto __END_TIME = std::chrono::high_resolution_clock::now();   \
  const float __DURATION =                                             \
      std::chrono::duration<float, std::chrono::milliseconds::period>( \
          __END_TIME - __START_TIME)                                   \
          .count();                                                    \
  std::cout << __MESSAGE << " took " << __DURATION << "ms" << std::endl;

#else

#define PROFILING_BEGIN(_)
#define PROFILING_END

#endif

#endif  // VULKAN_SRC_CORE_PROFILING_PROFILE_H_
