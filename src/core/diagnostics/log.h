#ifndef VULKAN_SRC_CORE_DIAGNOSTICS_LOG_H_
#define VULKAN_SRC_CORE_DIAGNOSTICS_LOG_H_

#include <string_view>

void BufferedLog(const std::string_view text);
void ImmediateLog(const std::string_view text);

#endif  // VULKAN_SRC_CORE_DIAGNOSTICS_LOG_H_
