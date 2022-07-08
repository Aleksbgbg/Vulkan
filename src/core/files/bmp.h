#ifndef SRC_CORE_FILES_BMP_H_
#define SRC_CORE_FILES_BMP_H_

#include <string_view>

#include "core/images/Bitmap.h"

namespace file {

Bitmap ReadBitmap(const std::string_view path);

}  // namespace file

#endif  // SRC_CORE_FILES_BMP_H_
