#ifndef VULKAN_SRC_GENERAL_LOGGING_LOG_H
#define VULKAN_SRC_GENERAL_LOGGING_LOG_H

#include <string_view>

void BufferedLog(const std::string_view text);
void ImmediateLog(const std::string_view text);

#endif  // VULKAN_SRC_GENERAL_LOGGING_LOG_H
