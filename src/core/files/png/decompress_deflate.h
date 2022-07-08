#ifndef SRC_CORE_FILES_PNG_DECOMPRESS_DEFLATE_H_
#define SRC_CORE_FILES_PNG_DECOMPRESS_DEFLATE_H_

#include "stream.h"

namespace file::png {

void DecompressDeflate(BitStreamReader& source, ByteStreamReadWriter& sink);

}

#endif  // SRC_CORE_FILES_PNG_DECOMPRESS_DEFLATE_H_
