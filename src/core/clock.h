#ifndef SRC_CORE_CLOCK_H_
#define SRC_CORE_CLOCK_H_

#include <chrono>

using default_clock = std::chrono::high_resolution_clock;
using time_point = std::chrono::time_point<default_clock>;

#endif  // SRC_CORE_CLOCK_H_
