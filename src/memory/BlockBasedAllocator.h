#ifndef VULKAN_SRC_MEMORY_BLOCKBASEDALLOCATOR_H_
#define VULKAN_SRC_MEMORY_BLOCKBASEDALLOCATOR_H_

#include <memory>
#include <vector>

#include "util/types.h"

class BlockBasedAllocator {
 public:
  struct Block {
    friend BlockBasedAllocator;

   public:
    void* memory;

   private:
    u32 index;
  };

  BlockBasedAllocator();
  BlockBasedAllocator(std::unique_ptr<u8[]> memory, const u64 memorySize,
                      const u32 blockSize);

  Block ConsumeBlock();
  void ReturnBlock(const Block block);

 private:
  struct Link {
    u32 index;
    Link* next;
  };

  std::unique_ptr<u8[]> memory_;
  u32 blockSize_;
  u32 allocatedBlocks_;
  u32 nextIndexToAssign_;
  u32 totalBlocks_;
  u8* blockMemory_;
  Link* allBlocks_;
  Link* firstFreeBlock_;
};

#endif  // VULKAN_SRC_MEMORY_BLOCKBASEDALLOCATOR_H_
