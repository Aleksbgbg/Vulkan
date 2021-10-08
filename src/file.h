#ifndef VULKAN_SRC_FILE_H
#define VULKAN_SRC_FILE_H

#include <vector>

#include "types.h"

std::vector<u8> ReadFile(const char* const filename);

void WriteFile(const char* const filename, const std::vector<u8>& data);

#endif  // VULKAN_SRC_FILE_H
