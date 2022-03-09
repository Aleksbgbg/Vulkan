#include "decompress_zlib.h"

#include <stdexcept>

#include "core/math/Range.h"
#include "core/math/bytes.h"
#include "decompress_deflate.h"

namespace file::png {

void DecompressZlib(ByteStreamReader& source, ByteStreamReadWriter& sink) {
  const u8 compressionMethodAndFlags = source.ReadByte();
  const u8 flags = source.ReadByte();

  const u8 compressionMethod =
      ExtractBytes(compressionMethodAndFlags, Range(1u, 4u));
  const bool dictionaryPresent = ExtractBytes(flags, Range(6u, 6u));

  if (compressionMethod != 8) {
    throw std::runtime_error("Invalid Zlib compression method.");
  }

  if (dictionaryPresent) {
    throw std::runtime_error("Unsupported Zlib dictionary");
  }

  BitStreamReader bitStreamReader = source.AsBitStream();
  DecompressDeflate(bitStreamReader, sink);
}

}  // namespace file::png
