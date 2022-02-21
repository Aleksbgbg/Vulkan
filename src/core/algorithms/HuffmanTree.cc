#include "HuffmanTree.h"

#include <cmath>
#include <queue>
#include <unordered_map>

void HuffmanNode::Insert(HuffmanNode& tree, const u32 code, const u32 value,
                         const u32 bits) {
  HuffmanNode* node = &tree;

  for (i32 bitIndex = bits - 1; bitIndex >= 0; --bitIndex) {
    const bool bitValue = (code >> bitIndex) & 0b1;

    std::unique_ptr<HuffmanNode>& branch = node->branches[bitValue];

    if (branch == nullptr) {
      branch = std::make_unique<HuffmanNode>();
    }

    node = branch.get();
  }

  node->value = value;
}

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

HuffmanNode HuffmanNode::BuildTree(const std::vector<u32>& alphabet,
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

  HuffmanNode huffmanTree;

  for (const u32 bitLength : bitLengths) {
    if (bitLength != 0) {
      const u32 code = nextCodeForBitLength[bitLength]++;
      const u32 value = alphabetSymbolForBitLength[bitLength].top();

      Insert(huffmanTree, code, value, bitLength);
      alphabetSymbolForBitLength[bitLength].pop();
    }
  }

  return huffmanTree;
}

const HuffmanNode* HuffmanNode::VisitSubNode(const bool nodeValue) const {
  return branches[static_cast<u32>(nodeValue)].get();
}

bool HuffmanNode::IsLeafNode() const {
  return (branches[0] == nullptr) && (branches[1] == nullptr);
}

u32 HuffmanNode::GetValue() const {
  return value;
}
