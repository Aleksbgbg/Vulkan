#ifndef SRC_CORE_FILES_PNG_HUFFMANCODE_H_
#define SRC_CORE_FILES_PNG_HUFFMANCODE_H_

#include <vector>

#include "core/types.h"
#include "stream.h"

class HuffmanCode {
 private:
  struct DecodeResult {
    u32 value;
    u32 bits;
  };

 public:
  static HuffmanCode Build(const std::vector<u32>& alphabet,
                           const std::vector<u32>& bitLengths);

  u32 DecodeNextValue(BitStreamReader& bits) const;

 private:
  HuffmanCode(std::vector<DecodeResult> decodeResultForCode, u32 minBits);

 private:
  std::vector<DecodeResult> decodeResultForCode_;
  u32 minBits_;
};

#endif  // SRC_CORE_FILES_PNG_HUFFMANCODE_H_
