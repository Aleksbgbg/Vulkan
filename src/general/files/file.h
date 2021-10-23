#ifndef VULKAN_SRC_FILE_H
#define VULKAN_SRC_FILE_H

#include <string_view>
#include <vector>

#include "util/types.h"

namespace file {

std::vector<u8> ReadFile(const std::string_view filename);

void WriteFile(const std::string_view filename, const std::vector<u8>& data);

}  // namespace file

#endif  // VULKAN_SRC_FILE_H
