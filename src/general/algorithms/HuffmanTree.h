#ifndef VULKAN_SRC_GENERAL_ALGORITHMS_HUFFMANTREE_H
#define VULKAN_SRC_GENERAL_ALGORITHMS_HUFFMANTREE_H

#include <array>
#include <memory>
#include <vector>

#include "util/types.h"

class HuffmanNode {
 public:
  static HuffmanNode BuildTree(const std::vector<u32>& alphabet,
                               const std::vector<u32>& bitLengths);

  const HuffmanNode* VisitSubNode(const bool nodeValue) const;

  bool IsLeafNode() const;
  u32 GetValue() const;

 private:
  static void Insert(HuffmanNode& tree, const u32 code, const u32 value,
                     const u32 bits);

 private:
  std::array<std::unique_ptr<HuffmanNode>, 2> branches;
  u32 value;
};

#endif  // VULKAN_SRC_GENERAL_ALGORITHMS_HUFFMANTREE_H
