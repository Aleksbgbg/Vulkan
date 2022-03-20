#ifndef VULKAN_SRC_CORE_DIAGNOSTICS_ERRORREPORT_H_
#define VULKAN_SRC_CORE_DIAGNOSTICS_ERRORREPORT_H_

#include <string_view>

struct ErrorReport {
  std::string_view location;
  std::string_view explanation;
};

#define AS_STRING_(VALUE) #VALUE
#define AS_STRING(VALUE) AS_STRING_(VALUE)
#define __LINE_STRING__ AS_STRING(__LINE__)

#define CREATE_ERROR_REPORT(EXPLANATION)                                   \
  ErrorReport {                                                            \
    .location = __FILE__ ":" __LINE_STRING__, .explanation = (EXPLANATION) \
  }

#endif  // VULKAN_SRC_CORE_DIAGNOSTICS_ERRORREPORT_H_
