#include "BlockBasedAllocator.h"

#include <stdexcept>

BlockBasedAllocator::BlockBasedAllocator() {}

BlockBasedAllocator::BlockBasedAllocator(std::unique_ptr<u8[]> memory,
                                         const u64 memorySize,
                                         const u32 blockSize)
    : memory_(std::move(memory)),
      blockSize_(blockSize),
      allocatedBlocks_(0),
      nextIndexToAssign_(1),
      totalBlocks_(memorySize / (blockSize + sizeof(Link))),
      blockMemory_(memory_.get() + sizeof(Link) * totalBlocks_),
      allBlocks_(new (memory_.get()) Link[totalBlocks_]),
      firstFreeBlock_(allBlocks_) {}

BlockBasedAllocator::Block BlockBasedAllocator::ConsumeBlock() {
  if (allocatedBlocks_ > totalBlocks_) {
    throw std::runtime_error(
        "BlockBasedAllocator has no more blocks available.");
  }
  ++allocatedBlocks_;

  Link* link = firstFreeBlock_;

  if (link->next == nullptr) {
    link->index = nextIndexToAssign_;
    link->next = allBlocks_ + (nextIndexToAssign_ + 1);
    ++nextIndexToAssign_;
  }

  firstFreeBlock_ = link->next;

  const u32 index = link->index;

  void* const memory = blockMemory_ + (blockSize_ * index);

  Block block;
  block.memory = memory;
  block.index = index;
  return block;
}

void BlockBasedAllocator::ReturnBlock(const BlockBasedAllocator::Block block) {
  Link* link = allBlocks_ + block.index;
  link->next = firstFreeBlock_;
  firstFreeBlock_ = link;

  --allocatedBlocks_;
}
