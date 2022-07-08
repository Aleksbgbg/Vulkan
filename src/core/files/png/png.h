#ifndef SRC_CORE_FILES_PNG_PNG_H_
#define SRC_CORE_FILES_PNG_PNG_H_

#include <string_view>

#include "core/images/Bitmap.h"

namespace file {

Bitmap ReadPng(const std::string_view path);

}  // namespace file

#endif  // SRC_CORE_FILES_PNG_PNG_H_
