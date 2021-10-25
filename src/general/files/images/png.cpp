#include "png.h"

#include <array>
#include <stdexcept>
#include <string>

#include "general/algorithms/HuffmanTree.h"
#include "general/files/file.h"

namespace file {

#define BYTES_TO_INT(bytes)                                            \
  ((((bytes)[0]) << 24) | (((bytes)[1]) << 16) | (((bytes)[2]) << 8) | \
   ((bytes)[3]))

struct LengthCodeInfo {
  u32 bits;
  u32 baseLength;
};

struct DistanceCodeInfo {
  u32 bits;
  u32 baseDistance;
};

static constexpr u32 LiteralAndLengthCodeCount = 288;
static constexpr u32 DistanceCodeCount = 32;

static constexpr u32 LiteralCodeUpperBound = 256;
static constexpr u32 EndCode = 256;

static constexpr LengthCodeInfo LengthCodes[] = {
    LengthCodeInfo{.bits = 0, .baseLength = 3},
    LengthCodeInfo{.bits = 0, .baseLength = 4},
    LengthCodeInfo{.bits = 0, .baseLength = 5},
    LengthCodeInfo{.bits = 0, .baseLength = 6},
    LengthCodeInfo{.bits = 0, .baseLength = 7},
    LengthCodeInfo{.bits = 0, .baseLength = 8},
    LengthCodeInfo{.bits = 0, .baseLength = 9},
    LengthCodeInfo{.bits = 0, .baseLength = 10},
    LengthCodeInfo{.bits = 1, .baseLength = 11},
    LengthCodeInfo{.bits = 1, .baseLength = 13},
    LengthCodeInfo{.bits = 1, .baseLength = 15},
    LengthCodeInfo{.bits = 1, .baseLength = 17},
    LengthCodeInfo{.bits = 2, .baseLength = 19},
    LengthCodeInfo{.bits = 2, .baseLength = 23},
    LengthCodeInfo{.bits = 2, .baseLength = 27},
    LengthCodeInfo{.bits = 2, .baseLength = 31},
    LengthCodeInfo{.bits = 3, .baseLength = 35},
    LengthCodeInfo{.bits = 3, .baseLength = 43},
    LengthCodeInfo{.bits = 3, .baseLength = 51},
    LengthCodeInfo{.bits = 3, .baseLength = 59},
    LengthCodeInfo{.bits = 4, .baseLength = 67},
    LengthCodeInfo{.bits = 4, .baseLength = 83},
    LengthCodeInfo{.bits = 4, .baseLength = 99},
    LengthCodeInfo{.bits = 4, .baseLength = 115},
    LengthCodeInfo{.bits = 5, .baseLength = 131},
    LengthCodeInfo{.bits = 5, .baseLength = 163},
    LengthCodeInfo{.bits = 5, .baseLength = 195},
    LengthCodeInfo{.bits = 5, .baseLength = 227},
    LengthCodeInfo{.bits = 0, .baseLength = 258}};

static constexpr DistanceCodeInfo DistanceCodes[] = {
    DistanceCodeInfo{.bits = 0, .baseDistance = 1},
    DistanceCodeInfo{.bits = 0, .baseDistance = 2},
    DistanceCodeInfo{.bits = 0, .baseDistance = 3},
    DistanceCodeInfo{.bits = 0, .baseDistance = 4},
    DistanceCodeInfo{.bits = 1, .baseDistance = 5},
    DistanceCodeInfo{.bits = 1, .baseDistance = 7},
    DistanceCodeInfo{.bits = 2, .baseDistance = 9},
    DistanceCodeInfo{.bits = 2, .baseDistance = 13},
    DistanceCodeInfo{.bits = 3, .baseDistance = 17},
    DistanceCodeInfo{.bits = 3, .baseDistance = 25},
    DistanceCodeInfo{.bits = 4, .baseDistance = 33},
    DistanceCodeInfo{.bits = 4, .baseDistance = 49},
    DistanceCodeInfo{.bits = 5, .baseDistance = 65},
    DistanceCodeInfo{.bits = 5, .baseDistance = 97},
    DistanceCodeInfo{.bits = 6, .baseDistance = 129},
    DistanceCodeInfo{.bits = 6, .baseDistance = 193},
    DistanceCodeInfo{.bits = 7, .baseDistance = 257},
    DistanceCodeInfo{.bits = 7, .baseDistance = 385},
    DistanceCodeInfo{.bits = 8, .baseDistance = 513},
    DistanceCodeInfo{.bits = 8, .baseDistance = 769},
    DistanceCodeInfo{.bits = 9, .baseDistance = 1025},
    DistanceCodeInfo{.bits = 9, .baseDistance = 1537},
    DistanceCodeInfo{.bits = 10, .baseDistance = 2049},
    DistanceCodeInfo{.bits = 10, .baseDistance = 3073},
    DistanceCodeInfo{.bits = 11, .baseDistance = 4097},
    DistanceCodeInfo{.bits = 11, .baseDistance = 6145},
    DistanceCodeInfo{.bits = 12, .baseDistance = 8193},
    DistanceCodeInfo{.bits = 12, .baseDistance = 12289},
    DistanceCodeInfo{.bits = 13, .baseDistance = 16385},
    DistanceCodeInfo{.bits = 13, .baseDistance = 24577}};

LengthCodeInfo LengthCodeInfoFrom(const u32 code) {
  return LengthCodes[code - 257];
}

DistanceCodeInfo DistanceCodeInfoFrom(const u32 distance) {
  return DistanceCodes[distance];
}

class BitStreamReader {
 public:
  BitStreamReader(const u8* const byteStream)
      : byteStream(byteStream), bitPosition(0) {}

  bool ReadBit() {
    const u32 byteIndex = bitPosition / 8;
    const u32 bitIndex = bitPosition % 8;

    ++bitPosition;

    const u8 byteValue = byteStream[byteIndex];
    return (byteValue >> bitIndex) & 0b1;
  }

  u32 ReadBitsLsbFirst(const u32 bitCount) {
    u32 value = 0;

    for (u32 bitIndex = 0; bitIndex < bitCount; ++bitIndex) {
      value |= ReadBit() << bitIndex;
    }

    return value;
  }

 private:
  const u8* const byteStream;
  u32 bitPosition;
};

class ByteStreamReader {
 public:
  ByteStreamReader(const u8* const data) : byte(data) {}

  struct MarkedPosition {
   private:
    friend ByteStreamReader;
    MarkedPosition(const u8* const byte) : byte(byte) {}
    const u8* const byte;
  };

  const u8* ReadBytes(const u32 size) {
    const u8* const bytesStart = byte;
    byte += size;
    return bytesStart;
  }

  u8 ReadByte() {
    return *ReadBytes(1);
  }

  u32 ReadInt() {
    const u8* const bytes = ReadBytes(4);
    return BYTES_TO_INT(bytes);
  }

  MarkedPosition MarkPosition(const u32 offset) const {
    return MarkedPosition(byte + offset);
  }

  void Jump(const MarkedPosition position) {
    byte = position.byte;
  }

  BitStreamReader AsBitStream() const {
    return BitStreamReader(byte);
  }

 private:
  const u8* byte;
};

class ByteStreamReadWriter {
 public:
  ByteStreamReadWriter(u8* const data) : data(data), offset(0) {}

  u32 WrittenSize() const {
    return offset;
  }

  u32 ReadByte(const u32 index) const {
    return data[index];
  }

  void WriteByte(const u8 value) {
    data[offset] = value;
    ++offset;
  }

 private:
  u8* data;
  u32 offset;
};

class DynamicHuffmanCodeLengthReader {
 public:
  DynamicHuffmanCodeLengthReader(const HuffmanNode& tree,
                                 std::vector<u32>& codeLengths,
                                 BitStreamReader& bitStream)
      : decodeTree(tree), codeLengths(codeLengths), bitStream(bitStream) {}

  void Decode() {
    for (u32 codeIndex = 0; codeIndex < codeLengths.size(); ++codeIndex) {
      const HuffmanNode* node = &decodeTree;

      while (!node->IsLeafNode()) {
        node = node->VisitSubNode(bitStream.ReadBit());
      }

      const u32 value = node->GetValue();

      if (value == 16) {
        const u32 repeat = 3 + bitStream.ReadBitsLsbFirst(2);
        const u32 lastCode = codeLengths[codeIndex - 1];

        for (u32 iteration = 0; iteration < repeat; ++iteration) {
          codeLengths[codeIndex + iteration] = lastCode;
        }

        codeIndex += repeat - 1;
      } else if (value == 17) {
        const u32 repeat = 3 + bitStream.ReadBitsLsbFirst(3);

        for (u32 iteration = 0; iteration < repeat; ++iteration) {
          codeLengths[codeIndex + iteration] = 0;
        }

        codeIndex += repeat - 1;
      } else if (value == 18) {
        const u32 repeat = 11 + bitStream.ReadBitsLsbFirst(7);

        for (u32 iteration = 0; iteration < repeat; ++iteration) {
          codeLengths[codeIndex + iteration] = 0;
        }

        codeIndex += repeat - 1;
      } else {
        codeLengths[codeIndex] = node->GetValue();
      }
    }
  }

 private:
  const HuffmanNode& decodeTree;
  std::vector<u32>& codeLengths;
  BitStreamReader& bitStream;
};

class DeflateDecoder {
 public:
  DeflateDecoder(const BitStreamReader bitStream,
                 const ByteStreamReadWriter outputStream)
      : bitStream(bitStream), outputStream(outputStream) {}

  void Run() {
    while (true) {
      const BlockHeader blockHeader = ReadBlockHeader();

      if (blockHeader.compressionType == BlockHeader::CompressionType::None) {
        throw std::runtime_error("Unsupported uncompressed DEFLATE block");
      } else {
        std::vector<u32> literalAndLengthAlphabet(LiteralAndLengthCodeCount);
        std::vector<u32> literalAndLengthCodeLengths;

        std::vector<u32> distanceAlphabet(DistanceCodeCount);
        std::vector<u32> distanceCodeLengths;

        for (u32 i = 0; i < LiteralAndLengthCodeCount; ++i) {
          literalAndLengthAlphabet[i] = i;
        }

        for (u32 i = 0; i < DistanceCodeCount; ++i) {
          distanceAlphabet[i] = i;
        }

        if (blockHeader.compressionType ==
            BlockHeader::CompressionType::FixedHuffmanCodes) {
          literalAndLengthCodeLengths.resize(LiteralAndLengthCodeCount);
          distanceCodeLengths.resize(DistanceCodeCount);

          for (u32 i = 0; i <= 143; ++i) {
            literalAndLengthCodeLengths[i] = 8;
          }
          for (u32 i = 144; i <= 255; ++i) {
            literalAndLengthCodeLengths[i] = 9;
          }
          for (u32 i = 256; i <= 279; ++i) {
            literalAndLengthCodeLengths[i] = 7;
          }
          for (u32 i = 280; i <= 287; ++i) {
            literalAndLengthCodeLengths[i] = 8;
          }

          for (u32 i = 0; i < DistanceCodeCount; ++i) {
            distanceCodeLengths[i] = 5;
          }
        } else if (blockHeader.compressionType ==
                   BlockHeader::CompressionType::DynamicHuffmanCodes) {
          const u32 literalCodeCount =
              bitStream.ReadBitsLsbFirst(5) + 257;  // HLIT
          const u32 distanceCodeCount =
              bitStream.ReadBitsLsbFirst(5) + 1;  // HDIST
          const u32 codeLengthCodeCount =
              bitStream.ReadBitsLsbFirst(4) + 4;  // HCLEN

          literalAndLengthCodeLengths.resize(literalCodeCount);
          distanceCodeLengths.resize(distanceCodeCount);
          std::vector<u32> codeLengths(codeLengthCodeCount);

          for (u32 lengthIndex = 0; lengthIndex < codeLengthCodeCount;
               ++lengthIndex) {
            codeLengths[lengthIndex] = bitStream.ReadBitsLsbFirst(3);
          }

          const HuffmanNode codeLengthTree =
              HuffmanNode::BuildTree({16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4,
                                      12, 3, 13, 2, 14, 1, 15},
                                     codeLengths);

          DynamicHuffmanCodeLengthReader(codeLengthTree,
                                         literalAndLengthCodeLengths, bitStream)
              .Decode();
          DynamicHuffmanCodeLengthReader(codeLengthTree, distanceCodeLengths,
                                         bitStream)
              .Decode();
        }

        const HuffmanNode literalAndLengthTree = HuffmanNode::BuildTree(
            literalAndLengthAlphabet, literalAndLengthCodeLengths);

        const HuffmanNode distanceTree =
            HuffmanNode::BuildTree(distanceAlphabet, distanceCodeLengths);

        while (true) {
          const HuffmanNode* node = &literalAndLengthTree;

          while (!node->IsLeafNode()) {
            node = node->VisitSubNode(bitStream.ReadBit());
          }

          const u32 value = node->GetValue();

          if (value < LiteralCodeUpperBound) {
            outputStream.WriteByte(value);
          } else if (value == EndCode) {
            break;
          } else {
            const LengthCodeInfo lengthCodeInfo = LengthCodeInfoFrom(value);

            u32 length = lengthCodeInfo.baseLength;

            if (lengthCodeInfo.bits > 0) {
              length += bitStream.ReadBitsLsbFirst(lengthCodeInfo.bits);
            }

            node = &distanceTree;

            while (!node->IsLeafNode()) {
              node = node->VisitSubNode(bitStream.ReadBit());
            }

            const DistanceCodeInfo distanceCodeInfo =
                DistanceCodeInfoFrom(node->GetValue());

            u32 distance = distanceCodeInfo.baseDistance;

            if (distanceCodeInfo.bits > 0) {
              distance += bitStream.ReadBitsLsbFirst(distanceCodeInfo.bits);
            }

            Repeat(length, distance);
          }
        }
      }

      if (blockHeader.isFinal) {
        break;
      }
    }
  }

  void Repeat(const u32 length, const u32 distance) {
    const u32 size = outputStream.WrittenSize();
    const u32 offsetIndex = size - distance;
    const u32 maxOffset = size - offsetIndex;

    for (u32 index = 0; index < length; ++index) {
      outputStream.WriteByte(
          outputStream.ReadByte(offsetIndex + (index % maxOffset)));
    }
  }

 private:
  struct BlockHeader {
    enum class CompressionType {
      None = 0,
      FixedHuffmanCodes = 1,
      DynamicHuffmanCodes = 2
    };

    bool isFinal;
    CompressionType compressionType;
  };

  BlockHeader ReadBlockHeader() {
    return BlockHeader{
        .isFinal = bitStream.ReadBit(),
        .compressionType = static_cast<BlockHeader::CompressionType>(
            bitStream.ReadBitsLsbFirst(2))};
  }

 private:
  BitStreamReader bitStream;
  ByteStreamReadWriter outputStream;
};

void DecompressZlib(ByteStreamReader& streamReader,
                    const ByteStreamReadWriter outputStream) {
  const u8 compressionMethodAndFlags = streamReader.ReadByte();
  const u8 flags = streamReader.ReadByte();

  const u8 compressionMethod = compressionMethodAndFlags & 0b1111;
  const bool dictionaryPresent = (flags >> 5) & 0b1;

  if (compressionMethod != 8) {
    throw std::runtime_error("Invalid Zlib compression method.");
  }

  if (dictionaryPresent) {
    throw std::runtime_error("Unsupported Zlib dictionary");
  }

  DeflateDecoder(streamReader.AsBitStream(), outputStream).Run();
}

class UnboundedScanlineReadWriter {
 public:
  UnboundedScanlineReadWriter(Image& image) : image(image) {}

  void Copy(const u32 scanline, const u8* const data) const {
    std::memcpy(ScanlineData(scanline), data, image.scanlineSize);
  }

  void WriteByte(const u32 scanline, const i32 byte, const u8 value) const {
    ScanlineData(scanline)[byte] = value;
  }

  u8 ReadByte(const u32 scanline, const i32 byte) const {
    if (byte < 0) {
      return 0;
    }

    return ScanlineData(scanline)[byte];
  }

 private:
  u8* ScanlineData(const u32 scanline) const {
    return image.data.data() + (scanline * image.scanlineSize);
  }

 private:
  Image& image;
};

class PngDefilter {
 public:
  PngDefilter(Image& image, const std::vector<u8> filteredData)
      : image(image),
        filteredData(std::move(filteredData)),
        source(this->filteredData.data()),
        scanlineReadWriter(image) {}

  void Run() {
    for (u32 scanline = 0; scanline < image.height; ++scanline) {
      const u8 filterType = source[0];
      ++source;

      switch (filterType) {
        case 0:
          DecodeNoneFilter(scanline);
          break;

        case 1:
          DecodeSubFilter(scanline);
          break;

        case 3:
          DecodeAverageFilter(scanline);
          break;

        case 4:
          DecodePaethFilter(scanline);
          break;

        default:
          throw std::runtime_error("Unsupported PNG filter type on scanline (" +
                                   std::to_string(filterType) + ")");
      }

      source += image.scanlineSize;
    }
  }

 private:
  void DecodeNoneFilter(const u32 scanline) const {
    scanlineReadWriter.Copy(scanline, source);
  }

  void DecodeSubFilter(const u32 scanline) const {
    for (u32 byte = 0; byte < image.scanlineSize; ++byte) {
      const u8 outputByte = source[byte] + ReadPixelA(scanline, byte);
      scanlineReadWriter.WriteByte(scanline, byte, outputByte);
    }
  }

  void DecodeAverageFilter(const u32 scanline) const {
    for (u32 byte = 0; byte < image.scanlineSize; ++byte) {
      const u8 a = ReadPixelA(scanline, byte);
      const u8 b = ReadPixelB(scanline, byte);

      const u8 outputByte = source[byte] + static_cast<u8>((a + b) / 2.0f);

      scanlineReadWriter.WriteByte(scanline, byte, outputByte);
    }
  }

  void DecodePaethFilter(const u32 scanline) const {
    for (u32 byte = 0; byte < image.scanlineSize; ++byte) {
      const u8 a = ReadPixelA(scanline, byte);
      const u8 b = ReadPixelB(scanline, byte);
      const u8 c = ReadPixelC(scanline, byte);

      const u8 outputByte = source[byte] + PaethPredictor(a, b, c);

      scanlineReadWriter.WriteByte(scanline, byte, outputByte);
    }
  }

  u8 ReadPixelA(const u32 scanline, const u32 byte) const {
    return scanlineReadWriter.ReadByte(scanline, byte - image.bytesPerPixel);
  }

  u8 ReadPixelB(const u32 scanline, const u32 byte) const {
    return scanlineReadWriter.ReadByte(scanline - 1, byte);
  }

  u8 ReadPixelC(const u32 scanline, const u32 byte) const {
    return scanlineReadWriter.ReadByte(scanline - 1,
                                       byte - image.bytesPerPixel);
  }

  static u8 PaethPredictor(const u8 a, const u8 b, const u8 c) {
    const u8 p = a + b - c;

    const u8 pa = std::abs(p - a);
    const u8 pb = std::abs(p - b);
    const u8 pc = std::abs(p - c);

    if ((pa <= pb) && (pa <= pc)) {
      return a;
    }

    if (pb <= pc) {
      return b;
    }

    return c;
  }

 private:
  Image& image;
  std::vector<u8> filteredData;
  u8* source;
  UnboundedScanlineReadWriter scanlineReadWriter;
};

class PngReader {
 public:
  PngReader(const std::string_view path)
      : pngData(ReadFile(path)),
        streamReader(pngData.data()),
        image(),
        compressedData() {}

  Image Read() {
    if (!IsValidSignature()) {
      throw std::runtime_error("Not a valid PNG image");
    }

    while (true) {
      const Chunk chunk = ReadChunk();

      switch (chunk.type) {
        case ChunkType::UNKNOWN:
          break;

        case ChunkType::IHDR: {
          image.width = streamReader.ReadInt();
          image.height = streamReader.ReadInt();
          const u8 bitDepth = streamReader.ReadByte();
          const u8 colourType = streamReader.ReadByte();
          const u8 compressionMethod = streamReader.ReadByte();
          const u8 filterMethod = streamReader.ReadByte();
          const u8 interlaceMethod = streamReader.ReadByte();

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

          image.bytesPerPixel = (bitDepth / 8) * 4;
          image.scanlineSize = image.bytesPerPixel * image.width;
          image.size = image.scanlineSize * image.height;
          image.data.resize(image.size);
        } break;

        case ChunkType::PLTE:
          throw std::runtime_error("PNG PLTE chunk unsupported");

        case ChunkType::IDAT:
          ReadIdatChunk(chunk.length);
          break;

        case ChunkType::IEND:
          ByteStreamReader dataStreamReader(compressedData.data());
          std::vector<u8> filteredData((image.width + 1) * image.height *
                                       image.bytesPerPixel);
          ByteStreamReadWriter decompressedStreamReadWriter(
              filteredData.data());
          DecompressZlib(dataStreamReader, decompressedStreamReadWriter);
          PngDefilter(image, filteredData).Run();
          return image;
      }

      streamReader.Jump(chunk.end);
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
    const u8* const bytes = streamReader.ReadBytes(8);
    return bytes[0] == 137 && bytes[1] == 80 && bytes[2] == 78 &&
           bytes[3] == 71 && bytes[4] == 13 && bytes[5] == 10 &&
           bytes[6] == 26 && bytes[7] == 10;
  }

  Chunk ReadChunk() {
    const u32 length = streamReader.ReadInt();
    return Chunk{.length = length,
                 .type = DetermineChunkType(streamReader.ReadInt()),
                 .end = streamReader.MarkPosition(length + 4)};
  }

  static u32 ChunkNameToInt(const char* const name) {
    return BYTES_TO_INT(name);
  }

  static ChunkType DetermineChunkType(const u32 chunk) {
    if (chunk == ChunkNameToInt("IHDR")) {
      return ChunkType::IHDR;
    }

    if (chunk == ChunkNameToInt("PLTE")) {
      return ChunkType::PLTE;
    }

    if (chunk == ChunkNameToInt("IDAT")) {
      return ChunkType::IDAT;
    }

    if (chunk == ChunkNameToInt("IEND")) {
      return ChunkType::IEND;
    }

    return ChunkType::UNKNOWN;
  }

  void ReadIdatChunk(const u32 chunkLength) {
    const u32 sizeBeforeCurrentChunk = compressedData.size();

    compressedData.resize(sizeBeforeCurrentChunk + chunkLength);

    const u8* const bytes = streamReader.ReadBytes(chunkLength);
    for (u32 byte = 0; byte < chunkLength; ++byte) {
      compressedData[byte + sizeBeforeCurrentChunk] = bytes[byte];
    }
  }

 private:
  std::vector<u8> pngData;
  ByteStreamReader streamReader;
  std::vector<u8> compressedData;
  Image image;
};

Image ReadPng(const std::string_view path) {
  return PngReader(path).Read();
}

}  // namespace file
