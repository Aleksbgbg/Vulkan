#ifndef VULKAN_SRC_CORE_FILES_PNG_DECOMPRESS_DEFLATE_H_
#define VULKAN_SRC_CORE_FILES_PNG_DECOMPRESS_DEFLATE_H_

#include "stream.h"

namespace file::png {

void DecompressDeflate(BitStreamReader& source, ByteStreamReadWriter& sink);

}

#endif  // VULKAN_SRC_CORE_FILES_PNG_DECOMPRESS_DEFLATE_H_
