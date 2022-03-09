#ifndef VULKAN_SRC_CORE_FILES_PNG_DECOMPRESS_ZLIB_H_
#define VULKAN_SRC_CORE_FILES_PNG_DECOMPRESS_ZLIB_H_

#include "stream.h"

namespace file::png {

void DecompressZlib(ByteStreamReader& source, ByteStreamReadWriter& sink);

}  // namespace file::png

#endif  // VULKAN_SRC_CORE_FILES_PNG_DECOMPRESS_ZLIB_H_
