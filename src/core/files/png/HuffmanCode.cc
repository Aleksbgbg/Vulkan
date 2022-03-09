#include "HuffmanCode.h"

#include <cmath>
#include <queue>
#include <unordered_map>

#include "core/math/bits.h"

std::vector<u32> CountCodesPerBitLength(const std::vector<u32>& bitLengths,
                                        const u32 maxBitLength) {
  std::vector<u32> codeCountPerBitLength(maxBitLength + 1);

  for (const u32 bitLength : bitLengths) {
    ++codeCountPerBitLength[bitLength];
  }

  return codeCountPerBitLength;
}

std::vector<u32> CalculateSmallestCodePerBitLength(
    const std::vector<u32>& codeCountPerBitLength, const u32 maxBitLength) {
  std::vector<u32> nextCodeForBitLength(maxBitLength + 1);

  u32 code = 0;

  for (u32 bits = 1; bits <= maxBitLength; ++bits) {
    code = (code + codeCountPerBitLength[bits - 1]) << 1;
    nextCodeForBitLength[bits] = code;
  }

  return nextCodeForBitLength;
}

HuffmanCode HuffmanCode::Build(const std::vector<u32>& alphabet,
                               const std::vector<u32>& bitLengths) {
  using priority_queue =
      std::priority_queue<u32, std::vector<u32>, std::greater<u32>>;
  std::unordered_map<u32, priority_queue> alphabetSymbolForBitLength;

  for (u32 index = 0; index < std::min(alphabet.size(), bitLengths.size());
       ++index) {
    const u32 bitLength = bitLengths[index];

    if (!alphabetSymbolForBitLength.contains(bitLength)) {
      alphabetSymbolForBitLength.emplace(bitLength, priority_queue());
    }

    alphabetSymbolForBitLength[bitLength].push(alphabet[index]);
  }

  const u32 maxBitLength =
      *std::max_element(bitLengths.begin(), bitLengths.end());

  const std::vector<u32> codeCountPerBitLength =
      CountCodesPerBitLength(bitLengths, maxBitLength);

  std::vector<u32> nextCodeForBitLength =
      CalculateSmallestCodePerBitLength(codeCountPerBitLength, maxBitLength);

  u32 minBits = 0;
  std::vector<DecodeResult> decodeResultForCode(std::pow(2, maxBitLength + 1));

  for (const u32 bitLength : bitLengths) {
    if (bitLength != 0) {
      const u32 code = nextCodeForBitLength[bitLength]++;
      const u32 value = alphabetSymbolForBitLength[bitLength].top();

      if (minBits > bitLength) {
        minBits = bitLength;
      }

      for (u32 index = 0; index <= ValueOfBits(bitLength); ++index) {
        if (LowerBitsEqual(bitLength, index, code)) {
          decodeResultForCode[index] = {.value = value, .bits = bitLength};
        }
      }

      alphabetSymbolForBitLength[bitLength].pop();
    }
  }

  return HuffmanCode(std::move(decodeResultForCode), minBits);
}

HuffmanCode::HuffmanCode(std::vector<DecodeResult> decodeResultForCode,
                         const u32 minBits)
    : decodeResultForCode_(std::move(decodeResultForCode)), minBits_(minBits) {}

u32 HuffmanCode::DecodeNextValue(BitStreamReader& bits) const {
  u32 bitsRead = minBits_;
  u32 value = bits.ReadBitsMsbFirst(bitsRead);

  while (true) {
    const DecodeResult result = decodeResultForCode_[value];

    if (result.bits == bitsRead) {
      return result.value;
    }

    value <<= 1;
    value |= static_cast<u32>(bits.ReadBit());
    ++bitsRead;
  }
}
