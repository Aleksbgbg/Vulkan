#ifndef VULKAN_SRC_CORE_FILES_PNG_DEFILTER_H_
#define VULKAN_SRC_CORE_FILES_PNG_DEFILTER_H_

#include "core/images/Bitmap.h"
#include "core/types.h"

namespace file::png {

void Defilter(const u8* filteredPng, Bitmap& bitmap);

}  // namespace file::png

#endif  // VULKAN_SRC_CORE_FILES_PNG_DEFILTER_H_
