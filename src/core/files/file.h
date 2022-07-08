#ifndef SRC_CORE_FILES_FILE_H_
#define SRC_CORE_FILES_FILE_H_

#include <string_view>
#include <vector>

#include "core/types.h"

namespace file {

std::vector<u8> ReadFile(const std::string_view filename);

void WriteFile(const std::string_view filename, const std::string& data);
void WriteFile(const std::string_view filename, const std::vector<u8>& data);

}  // namespace file

#endif  // SRC_CORE_FILES_FILE_H_
