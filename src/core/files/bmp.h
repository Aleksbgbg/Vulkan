#ifndef VULKAN_SRC_CORE_FILES_BMP_H_
#define VULKAN_SRC_CORE_FILES_BMP_H_

#include <string_view>

#include "core/images/Bitmap.h"

namespace file {

Bitmap ReadBitmap(const std::string_view path);

}  // namespace file

#endif  // VULKAN_SRC_CORE_FILES_BMP_H_
