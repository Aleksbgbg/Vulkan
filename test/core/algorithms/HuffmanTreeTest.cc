#include <gtest/gtest.h>

#include "core/algorithms/HuffmanTree.h"

u32 FindHuffmanCode(const HuffmanNode& tree,
                    const std::vector<bool>& codeBits) {
  const HuffmanNode* node = &tree;

  i32 index = -1;

  while (!node->IsLeafNode()) {
    node = node->VisitSubNode(codeBits[++index]);
  }

  return node->GetValue();
}

TEST(HuffmanTreeTest, EncodesSimpleAlphabetAndBitLengths) {
  const std::vector<u32> alphabet = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
  const std::vector<u32> bitLengths = {3, 3, 3, 3, 3, 2, 4, 4};

  const HuffmanNode huffmanTree = HuffmanNode::BuildTree(alphabet, bitLengths);

  EXPECT_EQ('A', FindHuffmanCode(huffmanTree, {0, 1, 0}));
  EXPECT_EQ('B', FindHuffmanCode(huffmanTree, {0, 1, 1}));
  EXPECT_EQ('C', FindHuffmanCode(huffmanTree, {1, 0, 0}));
  EXPECT_EQ('D', FindHuffmanCode(huffmanTree, {1, 0, 1}));
  EXPECT_EQ('E', FindHuffmanCode(huffmanTree, {1, 1, 0}));
  EXPECT_EQ('F', FindHuffmanCode(huffmanTree, {0, 0}));
  EXPECT_EQ('G', FindHuffmanCode(huffmanTree, {1, 1, 1, 0}));
  EXPECT_EQ('H', FindHuffmanCode(huffmanTree, {1, 1, 1, 1}));
}

TEST(HuffmanTreeTest, SortsAlphabetLexicographically) {
  const std::vector<u32> alphabet = {16, 17, 18, 0, 8,  7, 9,  6, 10, 5,
                                     11, 4,  12, 3, 13, 2, 14, 1, 15};
  const std::vector<u32> bitLengths = {2, 6, 0, 5, 3, 5, 3, 6, 3,
                                       6, 3, 5, 5, 5, 0, 6, 5};

  const HuffmanNode huffmanTree = HuffmanNode::BuildTree(alphabet, bitLengths);

  EXPECT_EQ(11, FindHuffmanCode(huffmanTree, {1, 0, 1}));
  EXPECT_EQ(14, FindHuffmanCode(huffmanTree, {1, 1, 1, 0, 1}));
  EXPECT_EQ(0, FindHuffmanCode(huffmanTree, {1, 1, 0, 0, 0}));
  EXPECT_EQ(12, FindHuffmanCode(huffmanTree, {1, 1, 1, 0, 0}));
  EXPECT_EQ(17, FindHuffmanCode(huffmanTree, {1, 1, 1, 1, 1, 1}));
}
