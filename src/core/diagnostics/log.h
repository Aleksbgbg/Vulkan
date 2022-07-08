#ifndef SRC_CORE_DIAGNOSTICS_LOG_H_
#define SRC_CORE_DIAGNOSTICS_LOG_H_

#include <string_view>

void BufferedLog(const std::string_view text);
void ImmediateLog(const std::string_view text);

#endif  // SRC_CORE_DIAGNOSTICS_LOG_H_
