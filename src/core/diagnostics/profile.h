#ifndef SRC_CORE_DIAGNOSTICS_PROFILE_H_
#define SRC_CORE_DIAGNOSTICS_PROFILE_H_

#include <chrono>

#include "core/strings/format.h"
#include "log.h"
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
  ImmediateLog(Format(__MESSAGE, " took ", __DURATION, "ms"));

#else

#define PROFILING_BEGIN(_)
#define PROFILING_END

#endif

#endif  // SRC_CORE_DIAGNOSTICS_PROFILE_H_
