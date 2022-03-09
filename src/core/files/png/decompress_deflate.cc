#include "decompress_deflate.h"

#include <stdexcept>
#include <vector>

#include "HuffmanCode.h"
#include "core/math/Range.h"
#include "core/types.h"
#include "stream.h"

namespace {

enum class CompressionType {
  None = 0,
  FixedHuffmanCodes = 1,
  DynamicHuffmanCodes = 2,
  Invalid = 3,
};

struct BlockHeader {
  bool isFinalBlock;
  CompressionType blockCompression;
};

struct BitCode {
  u32 baseValue;
  u32 extraBits;
};

static constexpr u32 kLiteralAndLengthCodeCount = 288;
static constexpr u32 kDistanceCodeCount = 32;

static constexpr u32 kEndCode = 256;

static constexpr u32 kLiteralCodeCountOffset = 257;
static constexpr u32 kDistanceCodeCountOffset = 1;
static constexpr u32 kCodeLengthCodeCountOffset = 4;

static constexpr BitCode kLengthBitCodes[] = {
    {.baseValue = 3, .extraBits = 0},   {.baseValue = 4, .extraBits = 0},
    {.baseValue = 5, .extraBits = 0},   {.baseValue = 6, .extraBits = 0},
    {.baseValue = 7, .extraBits = 0},   {.baseValue = 8, .extraBits = 0},
    {.baseValue = 9, .extraBits = 0},   {.baseValue = 10, .extraBits = 0},
    {.baseValue = 11, .extraBits = 1},  {.baseValue = 13, .extraBits = 1},
    {.baseValue = 15, .extraBits = 1},  {.baseValue = 17, .extraBits = 1},
    {.baseValue = 19, .extraBits = 2},  {.baseValue = 23, .extraBits = 2},
    {.baseValue = 27, .extraBits = 2},  {.baseValue = 31, .extraBits = 2},
    {.baseValue = 35, .extraBits = 3},  {.baseValue = 43, .extraBits = 3},
    {.baseValue = 51, .extraBits = 3},  {.baseValue = 59, .extraBits = 3},
    {.baseValue = 67, .extraBits = 4},  {.baseValue = 83, .extraBits = 4},
    {.baseValue = 99, .extraBits = 4},  {.baseValue = 115, .extraBits = 4},
    {.baseValue = 131, .extraBits = 5}, {.baseValue = 163, .extraBits = 5},
    {.baseValue = 195, .extraBits = 5}, {.baseValue = 227, .extraBits = 5},
    {.baseValue = 258, .extraBits = 0}};

static constexpr BitCode kDistanceBitCodes[] = {
    {.baseValue = 1, .extraBits = 0},
    {.baseValue = 2, .extraBits = 0},
    {.baseValue = 3, .extraBits = 0},
    {.baseValue = 4, .extraBits = 0},
    {.baseValue = 5, .extraBits = 1},
    {.baseValue = 7, .extraBits = 1},
    {.baseValue = 9, .extraBits = 2},
    {.baseValue = 13, .extraBits = 2},
    {.baseValue = 17, .extraBits = 3},
    {.baseValue = 25, .extraBits = 3},
    {.baseValue = 33, .extraBits = 4},
    {.baseValue = 49, .extraBits = 4},
    {.baseValue = 65, .extraBits = 5},
    {.baseValue = 97, .extraBits = 5},
    {.baseValue = 129, .extraBits = 6},
    {.baseValue = 193, .extraBits = 6},
    {.baseValue = 257, .extraBits = 7},
    {.baseValue = 385, .extraBits = 7},
    {.baseValue = 513, .extraBits = 8},
    {.baseValue = 769, .extraBits = 8},
    {.baseValue = 1025, .extraBits = 9},
    {.baseValue = 1537, .extraBits = 9},
    {.baseValue = 2049, .extraBits = 10},
    {.baseValue = 3073, .extraBits = 10},
    {.baseValue = 4097, .extraBits = 11},
    {.baseValue = 6145, .extraBits = 11},
    {.baseValue = 8193, .extraBits = 12},
    {.baseValue = 12289, .extraBits = 12},
    {.baseValue = 16385, .extraBits = 13},
    {.baseValue = 24577, .extraBits = 13}};

std::vector<u32> CreateAlphabet(const u32 size) {
  std::vector<u32> alphabet(size);

  for (u32 index = 0; index < size; ++index) {
    alphabet[index] = index;
  }

  return alphabet;
}

void RepeatInRange(const u32 value, const Range<u32> range,
                   std::vector<u32>& vector) {
  for (u32 index = range.min; index <= range.max; ++index) {
    vector[index] = value;
  }
}

std::vector<u32> CreateFixedLiteralAndLengthCodeLengths() {
  std::vector<u32> codeLengths(kLiteralAndLengthCodeCount);
  RepeatInRange(8, Range(0u, 143u), codeLengths);
  RepeatInRange(9, Range(144u, 255u), codeLengths);
  RepeatInRange(7, Range(256u, 279u), codeLengths);
  RepeatInRange(8, Range(280u, 287u), codeLengths);
  return codeLengths;
}

std::vector<u32> CreateFixedDistanceCodeLengths() {
  std::vector<u32> codeLengths(kDistanceCodeCount);
  RepeatInRange(5, Range(0u, kDistanceCodeCount - 1), codeLengths);
  return codeLengths;
}

static const std::vector<u32> kLiteralAndLengthAlphabet =
    CreateAlphabet(kLiteralAndLengthCodeCount);
static const std::vector<u32> kDistanceAlphabet =
    CreateAlphabet(kDistanceCodeCount);
static constexpr std::initializer_list<u32> kCodeLengthAlphabet = {
    16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};

static const HuffmanCode kFixedLiteralAndLengthCode = HuffmanCode::Build(
    kLiteralAndLengthAlphabet, CreateFixedLiteralAndLengthCodeLengths());
static const HuffmanCode kFixedDistanceCode = HuffmanCode::Build(
    kLiteralAndLengthAlphabet, CreateFixedDistanceCodeLengths());

bool IsLiteralCode(const u32 code) {
  return code < kEndCode;
}

bool IsEndCode(const u32 code) {
  return code == kEndCode;
}

BitCode AsLengthBitCode(const u32 lengthCode) {
  return kLengthBitCodes[lengthCode - (kEndCode + 1)];
}

BitCode AsDistanceBitCode(const u32 distanceCode) {
  return kDistanceBitCodes[distanceCode];
}

class DeflateDecoder {
 public:
  DeflateDecoder(BitStreamReader& source, ByteStreamReadWriter& sink)
      : source_(source), sink_(sink) {}

  void Run() {
    while (true) {
      const BlockHeader header = ReadBlockHeader();

      if (header.blockCompression == CompressionType::DynamicHuffmanCodes) {
        DecompressDynamicBlock();
      } else if (header.blockCompression ==
                 CompressionType::FixedHuffmanCodes) {
        DecompressFixedBlock();
      } else if (header.blockCompression == CompressionType::None) {
        throw std::runtime_error("Unsupported uncompressed DEFLATE block");
      } else {
        throw std::runtime_error("Invalid DEFLATE block");
      }

      if (header.isFinalBlock) {
        break;
      }
    }
  }

 private:
  void DecompressDynamicBlock() {
    const u32 literalCodeCount =
        kLiteralCodeCountOffset + source_.ReadBitsLsbFirst(5);
    const u32 distanceCodeCount =
        kDistanceCodeCountOffset + source_.ReadBitsLsbFirst(5);
    const u32 codeLengthCodeCount =
        kCodeLengthCodeCountOffset + source_.ReadBitsLsbFirst(4);

    std::vector<u32> codeLengths(codeLengthCodeCount);

    for (u32 lengthIndex = 0; lengthIndex < codeLengthCodeCount;
         ++lengthIndex) {
      codeLengths[lengthIndex] = source_.ReadBitsLsbFirst(3);
    }

    const HuffmanCode codeLengthCode =
        HuffmanCode::Build(kCodeLengthAlphabet, codeLengths);

    std::vector<u32> literalAndLengthCodeLengths(literalCodeCount);
    ReadDynamicCodeLengths(codeLengthCode, literalAndLengthCodeLengths);

    std::vector<u32> distanceCodeLengths(distanceCodeCount);
    ReadDynamicCodeLengths(codeLengthCode, distanceCodeLengths);

    DecompressStream(
        HuffmanCode::Build(kLiteralAndLengthAlphabet,
                           literalAndLengthCodeLengths),
        HuffmanCode::Build(kDistanceAlphabet, distanceCodeLengths));
  }

  void ReadDynamicCodeLengths(const HuffmanCode& codeLengthCode,
                              std::vector<u32>& codeLengths) {
    for (u32 codeIndex = 0; codeIndex < codeLengths.size(); ++codeIndex) {
      const u32 code = codeLengthCode.DecodeNextValue(source_);

      if (code < 16) {
        codeLengths[codeIndex] = code;
      } else {
        u32 repeatOffset;
        u32 repeatBitLength;
        u32 repeatValue;

        if (code == 16) {
          repeatOffset = 3;
          repeatBitLength = 2;
          repeatValue = codeLengths[codeIndex - 1];
        } else if (code == 17) {
          repeatOffset = 3;
          repeatBitLength = 3;
          repeatValue = 0;
        } else {
          repeatOffset = 11;
          repeatBitLength = 7;
          repeatValue = 0;
        }

        const u32 repeatCount =
            repeatOffset + source_.ReadBitsLsbFirst(repeatBitLength);
        const u32 repeatUntilIndex = codeIndex + repeatCount;

        for (; codeIndex < repeatUntilIndex; ++codeIndex) {
          codeLengths[codeIndex] = repeatValue;
        }

        --codeIndex;
      }
    }
  }

  void DecompressFixedBlock() {
    DecompressStream(kFixedLiteralAndLengthCode, kFixedDistanceCode);
  }

  void DecompressStream(const HuffmanCode& literalAndLengthCode,
                        const HuffmanCode& distanceCode) {
    while (true) {
      const u32 value = literalAndLengthCode.DecodeNextValue(source_);

      if (IsLiteralCode(value)) {
        sink_.WriteByte(value);
      } else if (IsEndCode(value)) {
        return;
      } else {
        DecompressLengthDistanceCode(distanceCode, value);
      }
    }
  }

  void DecompressLengthDistanceCode(const HuffmanCode& distanceCode,
                                    const u32 lengthCode) {
    const u32 length = ReadBitCode(AsLengthBitCode(lengthCode));
    const u32 distance =
        ReadBitCode(AsDistanceBitCode(distanceCode.DecodeNextValue(source_)));

    RepeatLastPattern(length, distance);
  }

  u32 ReadBitCode(const BitCode bitCode) {
    return bitCode.baseValue + source_.ReadBitsLsbFirst(bitCode.extraBits);
  }

  void RepeatLastPattern(const u32 length, const u32 distance) {
    const u32 size = sink_.WrittenSize();
    const u32 offsetIndex = size - distance;
    const u32 maxOffset = size - offsetIndex;

    for (u32 index = 0; index < length; ++index) {
      sink_.WriteByte(sink_.ReadByte(offsetIndex + (index % maxOffset)));
    }
  }

  BlockHeader ReadBlockHeader() {
    return {.isFinalBlock = source_.ReadBit(),
            .blockCompression =
                static_cast<CompressionType>(source_.ReadBitsLsbFirst(2))};
  }

 private:
  BitStreamReader& source_;
  ByteStreamReadWriter& sink_;
};

}  // namespace

namespace file::png {

void DecompressDeflate(BitStreamReader& source, ByteStreamReadWriter& sink) {
  DeflateDecoder(source, sink).Run();
}

}  // namespace file::png
