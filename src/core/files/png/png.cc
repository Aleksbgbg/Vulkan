#include "png.h"

#include <cstring>
#include <stdexcept>
#include <string_view>
#include <vector>

#include "core/files/file.h"
#include "core/images/Bitmap.h"
#include "core/math/bytes.h"
#include "core/types.h"
#include "decompress_zlib.h"
#include "defilter.h"
#include "stream.h"

namespace {

consteval u32 ChunkNameAsInt(const char* const bytes) {
  return BytesToInt(bytes);
}

}  // namespace

namespace file {

class PngReader {
 public:
  PngReader(std::vector<u8> pngData)
      : pngData_(std::move(pngData)),
        streamReader_(pngData_.data()),
        image_(),
        compressedData_() {}

  Bitmap Read() {
    if (!IsValidSignature()) {
      throw std::runtime_error("Not a valid PNG image");
    }

    while (true) {
      const Chunk chunk = ReadChunk();

      switch (chunk.type) {
        case ChunkType::UNKNOWN:
          break;

        case ChunkType::IHDR: {
          const u32 width = streamReader_.ReadInt();
          const u32 height = streamReader_.ReadInt();
          const u8 bitDepth = streamReader_.ReadByte();
          const u8 colourType = streamReader_.ReadByte();
          const u8 compressionMethod = streamReader_.ReadByte();
          const u8 filterMethod = streamReader_.ReadByte();
          const u8 interlaceMethod = streamReader_.ReadByte();

          if (bitDepth != 8) {
            throw std::runtime_error("Unsupported PNG bit depth");
          }

          if (colourType != static_cast<u8>(ColourType::TruecolorAlpha)) {
            throw std::runtime_error("Unsupported PNG colour type");
          }

          if (compressionMethod != 0) {
            throw std::runtime_error("Invalid PNG compression method");
          }

          if (filterMethod != 0) {
            throw std::runtime_error("Invalid PNG filter method");
          }

          if (interlaceMethod != 0) {
            throw std::runtime_error("Unsupported PNG interlace method");
          }

          image_ = Bitmap(width, height, BitsPerPixel(bitDepth * 4));
        } break;

        case ChunkType::PLTE:
          throw std::runtime_error("PNG PLTE chunk unsupported");

        case ChunkType::IDAT:
          ReadIdatChunk(chunk.length);
          break;

        case ChunkType::IEND:
          ByteStreamReader dataStreamReader(compressedData_.data());
          std::vector<u8> filteredData((image_.Width() + 1) * image_.Height() *
                                       image_.BytesPerPixel());
          ByteStreamReadWriter decompressedStreamReadWriter(
              filteredData.data());
          png::DecompressZlib(dataStreamReader, decompressedStreamReadWriter);
          png::Defilter(filteredData.data(), image_);
          return image_;
      }

      streamReader_.Jump(chunk.end);
    }
  }

 private:
  enum class ChunkType : u32 { UNKNOWN = 0, IHDR, PLTE, IDAT, IEND };

  enum class ColourType : u8 {
    Greyscale = 0,
    Truecolour = 2,
    IndexedColour = 3,
    GreyscaleAlpha = 4,
    TruecolorAlpha = 6
  };

  struct Chunk {
    u32 length;
    ChunkType type;
    ByteStreamReader::MarkedPosition end;
  };

  bool IsValidSignature() {
    const u8* const bytes = streamReader_.ReadBytes(8);
    return bytes[0] == 137 && bytes[1] == 80 && bytes[2] == 78 &&
           bytes[3] == 71 && bytes[4] == 13 && bytes[5] == 10 &&
           bytes[6] == 26 && bytes[7] == 10;
  }

  Chunk ReadChunk() {
    const u32 length = streamReader_.ReadInt();
    return {.length = length,
            .type = DetermineChunkType(streamReader_.ReadInt()),
            .end = streamReader_.MarkPosition(length + 4)};
  }

  static ChunkType DetermineChunkType(const u32 chunk) {
    if (chunk == ChunkNameAsInt("IHDR")) {
      return ChunkType::IHDR;
    }

    if (chunk == ChunkNameAsInt("PLTE")) {
      return ChunkType::PLTE;
    }

    if (chunk == ChunkNameAsInt("IDAT")) {
      return ChunkType::IDAT;
    }

    if (chunk == ChunkNameAsInt("IEND")) {
      return ChunkType::IEND;
    }

    return ChunkType::UNKNOWN;
  }

  void ReadIdatChunk(const u32 chunkLength) {
    const u32 sizeBeforeCurrentChunk = compressedData_.size();

    compressedData_.resize(sizeBeforeCurrentChunk + chunkLength);

    const u8* const bytes = streamReader_.ReadBytes(chunkLength);
    std::memcpy(compressedData_.data() + sizeBeforeCurrentChunk, bytes,
                chunkLength);
  }

 private:
  std::vector<u8> pngData_;
  ByteStreamReader streamReader_;
  std::vector<u8> compressedData_;
  Bitmap image_;
};

Bitmap ReadPng(std::vector<u8> pngData) {
  return PngReader(std::move(pngData)).Read();
}

}  // namespace file
