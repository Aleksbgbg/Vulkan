#ifndef SRC_CORE_DIAGNOSTICS_LOG_H_
#define SRC_CORE_DIAGNOSTICS_LOG_H_

#include <string_view>

void BufferedLog(std::string_view text);
void ImmediateLog(std::string_view text);

#endif  // SRC_CORE_DIAGNOSTICS_LOG_H_
