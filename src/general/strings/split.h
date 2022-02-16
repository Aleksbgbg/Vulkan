#ifndef VULKAN_SRC_GENERAL_STRINGS_SPLIT_H_
#define VULKAN_SRC_GENERAL_STRINGS_SPLIT_H_

#include <string>
#include <string_view>
#include <vector>

std::vector<std::string> Split(std::string_view string,
                               std::string_view delimiter);

#endif  // VULKAN_SRC_GENERAL_STRINGS_SPLIT_H_
