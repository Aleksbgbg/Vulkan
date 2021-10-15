#include <gtest/gtest.h>

#include <queue>
#include <vector>

#include "memory/Allocator.h"
#include "memory/DeviceHeap.h"
#include "memory/MemoryAllocation.h"

static constexpr u64 ONE_MEGABYTE = 1ull * 1024ull * 1024ull;
static constexpr u64 TWO_MEGABYTES = 2ull * 1024ull * 1024ull;
static constexpr u64 THREE_MEGABYTES = 3ull * 1024ull * 1024ull;
static constexpr u64 FOUR_MEGABYTES = 4ull * 1024ull * 1024ull;

class CopyCountingMemoryObject : public MemoryObject {
 public:
  CopyCountingMemoryObject(u32* aliveCopies) : aliveCopies(aliveCopies) {
    ++(*aliveCopies);
  }

  CopyCountingMemoryObject(CopyCountingMemoryObject& other) noexcept
      : aliveCopies(other.aliveCopies) {
    ++(*aliveCopies);
  }

  CopyCountingMemoryObject(CopyCountingMemoryObject&&) = delete;

  ~CopyCountingMemoryObject() {
    --(*aliveCopies);
  }

  CopyCountingMemoryObject& operator=(CopyCountingMemoryObject& other) = delete;
  CopyCountingMemoryObject& operator=(CopyCountingMemoryObject&&) = delete;

  u32* aliveCopies;
};

class MemoryObjectWithId : public MemoryObject {
 public:
  MemoryObjectWithId() : MemoryObjectWithId(0) {}
  MemoryObjectWithId(u32 id) : id(id) {}

  MemoryObjectWithId(MemoryObjectWithId& other) noexcept : id(other.id) {}

  MemoryObjectWithId(MemoryObjectWithId&&) = delete;

  MemoryObjectWithId& operator=(MemoryObjectWithId& other) = delete;
  MemoryObjectWithId& operator=(MemoryObjectWithId&&) = delete;

  u32 id;
};

struct Allocation {
  u64 size;
};

class TestAllocator : public Allocator {
 public:
  TestAllocator() = default;
  TestAllocator(std::queue<MemoryObject*> objectsToAllocate)
      : objectsToAllocate(objectsToAllocate) {}

  std::unique_ptr<MemoryObject> Allocate(const VkDeviceSize size) override {
    allocations.emplace_back(Allocation{.size = size});

    if (objectsToAllocate.empty()) {
      return nullptr;
    }

    MemoryObject* allocatedMemoryObject = objectsToAllocate.front();
    objectsToAllocate.pop();
    return std::unique_ptr<MemoryObject>(allocatedMemoryObject);
  }

  std::vector<Allocation> allocations;

 private:
  std::queue<MemoryObject*> objectsToAllocate;
};

TEST(DeviceMemoryAllocatorTest, ReservesRequestedMemory) {
  TestAllocator allocator;
  DeviceHeap heap(ONE_MEGABYTE, &allocator);

  const ReservedBlock reservedBlock =
      heap.ReserveMemory({.size = 128, .alignment = 1});

  ASSERT_EQ(0, reservedBlock.GetMemoryBlock().offset);
  ASSERT_EQ(128, reservedBlock.GetMemoryBlock().size);
}

TEST(DeviceMemoryAllocatorTest, HonoursConsecutiveReservations) {
  TestAllocator allocator;
  DeviceHeap heap(ONE_MEGABYTE, &allocator);

  const ReservedBlock firstReservedBlock =
      heap.ReserveMemory({.size = 128, .alignment = 1});
  const ReservedBlock lastReservedBlock =
      heap.ReserveMemory({.size = 64, .alignment = 1});

  ASSERT_EQ(128, lastReservedBlock.GetMemoryBlock().offset);
  ASSERT_EQ(64, lastReservedBlock.GetMemoryBlock().size);
}

TEST(DeviceMemoryAllocatorTest, FirstReservationCausesInitialAllocation) {
  TestAllocator allocator;
  DeviceHeap heap(THREE_MEGABYTES, &allocator);

  heap.ReserveMemory({.size = 128, .alignment = 1});

  ASSERT_EQ(1, allocator.allocations.size());
  ASSERT_EQ(THREE_MEGABYTES, allocator.allocations[0].size);
}

TEST(DeviceMemoryAllocatorTest,
     SubReservationsExceedingAllocationCauseFurtherAllocations) {
  TestAllocator allocator;
  DeviceHeap heap(FOUR_MEGABYTES, &allocator);

  const ReservedBlock reservedBlock1 =
      heap.ReserveMemory({.size = TWO_MEGABYTES, .alignment = 1});
  const ReservedBlock reservedBlock2 =
      heap.ReserveMemory({.size = ONE_MEGABYTE, .alignment = 1});
  const ReservedBlock reservedBlock3 =
      heap.ReserveMemory({.size = TWO_MEGABYTES, .alignment = 1});

  ASSERT_EQ(2, allocator.allocations.size());
}

TEST(DeviceMemoryAllocatorTest,
     SubReservationsNotExceedingAllocationDoNotCauseFurtherAllocations) {
  TestAllocator allocator;
  DeviceHeap heap(FOUR_MEGABYTES, &allocator);

  const ReservedBlock reservedBlock1 =
      heap.ReserveMemory({.size = TWO_MEGABYTES, .alignment = 1});
  const ReservedBlock reservedBlock2 =
      heap.ReserveMemory({.size = ONE_MEGABYTE, .alignment = 1});

  ASSERT_EQ(1, allocator.allocations.size());
}

TEST(DeviceMemoryAllocatorTest, AllocateEntireHeapPossible) {
  TestAllocator allocator;
  DeviceHeap heap(1, &allocator);

  const ReservedBlock reservedBlock =
      heap.ReserveMemory({.size = 1, .alignment = 1});

  ASSERT_EQ(0, reservedBlock.GetMemoryBlock().offset);
  ASSERT_EQ(1, reservedBlock.GetMemoryBlock().size);
  ASSERT_EQ(1, allocator.allocations.size());
}

TEST(DeviceMemoryAllocatorTest, ReleasedMemoryCanBeReserved) {
  TestAllocator allocator;
  DeviceHeap heap(THREE_MEGABYTES, &allocator);

  {
    const ReservedBlock reservedBlock =
        heap.ReserveMemory({.size = 128, .alignment = 1});
    const ReservedBlock reservedBlock2 =
        heap.ReserveMemory({.size = 64, .alignment = 1});
  }
  const ReservedBlock reservedBlock =
      heap.ReserveMemory({.size = 256, .alignment = 1});

  ASSERT_EQ(0, reservedBlock.GetMemoryBlock().offset);
  ASSERT_EQ(256, reservedBlock.GetMemoryBlock().size);
}

TEST(DeviceMemoryAllocatorTest, HeapDoesNotGetFragmented) {
  // Arrange
  TestAllocator allocator;
  DeviceHeap heap(6, &allocator);

  // Act
  const ReservedBlock* const reservedBlock1 =
      new ReservedBlock(heap.ReserveMemory({.size = 1, .alignment = 1}));
  const ReservedBlock* const reservedBlock2 =
      new ReservedBlock(heap.ReserveMemory({.size = 1, .alignment = 1}));
  const ReservedBlock* const reservedBlock3 =
      new ReservedBlock(heap.ReserveMemory({.size = 1, .alignment = 1}));
  const ReservedBlock* const reservedBlock4 =
      new ReservedBlock(heap.ReserveMemory({.size = 1, .alignment = 1}));
  const ReservedBlock* const reservedBlock5 =
      new ReservedBlock(heap.ReserveMemory({.size = 1, .alignment = 1}));
  const ReservedBlock* const reservedBlock6 =
      new ReservedBlock(heap.ReserveMemory({.size = 1, .alignment = 1}));

  delete reservedBlock2;
  delete reservedBlock1;
  delete reservedBlock5;
  delete reservedBlock6;
  delete reservedBlock3;
  delete reservedBlock4;

  const ReservedBlock wholeAllocation =
      heap.ReserveMemory({.size = 6, .alignment = 1});

  // Assert
  ASSERT_EQ(0, wholeAllocation.GetMemoryBlock().offset);
  ASSERT_EQ(6, wholeAllocation.GetMemoryBlock().size);
  ASSERT_EQ(1, allocator.allocations.size());
}

TEST(DeviceMemoryAllocatorTest, HeapDoesNotGetFragmentedForBigAllocations) {
  // Arrange
  TestAllocator allocator;
  DeviceHeap heap(ONE_MEGABYTE, &allocator);

  // Act
  const ReservedBlock* const reservedBlock1 =
      new ReservedBlock(heap.ReserveMemory({.size = 10, .alignment = 1}));
  const ReservedBlock* const reservedBlock2 = new ReservedBlock(
      heap.ReserveMemory({.size = 128000, .alignment = 65536}));
  const ReservedBlock* const reservedBlock3 =
      new ReservedBlock(heap.ReserveMemory({.size = 65000, .alignment = 1}));

  delete reservedBlock2;
  delete reservedBlock1;
  delete reservedBlock3;

  const ReservedBlock wholeAllocation =
      heap.ReserveMemory({.size = ONE_MEGABYTE, .alignment = 1});

  // Assert
  ASSERT_EQ(0, wholeAllocation.GetMemoryBlock().offset);
  ASSERT_EQ(ONE_MEGABYTE, wholeAllocation.GetMemoryBlock().size);
  ASSERT_EQ(1, allocator.allocations.size());
}

TEST(DeviceMemoryAllocatorTest, KeepsAllocationsAliveOnAllHeaps) {
  CopyCountingMemoryObject* const allocation1 =
      new CopyCountingMemoryObject(new u32());
  CopyCountingMemoryObject* const allocation2 =
      new CopyCountingMemoryObject(new u32());
  std::queue<MemoryObject*> objectsToAllocate;
  objectsToAllocate.emplace(allocation1);
  objectsToAllocate.emplace(allocation2);
  TestAllocator allocator(objectsToAllocate);
  DeviceHeap heap(1, &allocator);

  {
    const ReservedBlock reservedBlock1 =
        heap.ReserveMemory({.size = 1, .alignment = 1});
    const ReservedBlock reservedBlock2 =
        heap.ReserveMemory({.size = 1, .alignment = 1});
  }

  ASSERT_EQ(1, *allocation1->aliveCopies);
  ASSERT_EQ(1, *allocation2->aliveCopies);
}

TEST(DeviceMemoryAllocatorTest, DestroyesAllocationsWhenDestroyed) {
  u32* const allocation1AliveCopies = new u32(0);
  u32* const allocation2AliveCopies = new u32(0);
  CopyCountingMemoryObject* const allocation1 =
      new CopyCountingMemoryObject(allocation1AliveCopies);
  CopyCountingMemoryObject* const allocation2 =
      new CopyCountingMemoryObject(allocation2AliveCopies);
  std::queue<MemoryObject*> objectsToAllocate;
  objectsToAllocate.emplace(allocation1);
  objectsToAllocate.emplace(allocation2);
  TestAllocator allocator(objectsToAllocate);

  {
    DeviceHeap heap(1, &allocator);
    const ReservedBlock reservedBlock1 =
        heap.ReserveMemory({.size = 1, .alignment = 1});
    const ReservedBlock reservedBlock2 =
        heap.ReserveMemory({.size = 1, .alignment = 1});
  }

  ASSERT_EQ(0, *allocation1AliveCopies);
  ASSERT_EQ(0, *allocation2AliveCopies);
}

TEST(DeviceMemoryAllocatorTest, ReturnsAllocationsToCorrectHeaps) {
  // Arrange
  TestAllocator allocator;
  DeviceHeap heap(3, &allocator);

  // Act
  const ReservedBlock* const allocation1ReservedBlock1 =
      new ReservedBlock(heap.ReserveMemory({.size = 1, .alignment = 1}));
  const ReservedBlock* const allocation1ReservedBlock2 =
      new ReservedBlock(heap.ReserveMemory({.size = 1, .alignment = 1}));
  const ReservedBlock* const allocation1ReservedBlock3 =
      new ReservedBlock(heap.ReserveMemory({.size = 1, .alignment = 1}));
  const ReservedBlock* const allocation2ReservedBlock1 =
      new ReservedBlock(heap.ReserveMemory({.size = 1, .alignment = 1}));
  const ReservedBlock* const allocation2ReservedBlock2 =
      new ReservedBlock(heap.ReserveMemory({.size = 1, .alignment = 1}));
  const ReservedBlock* const allocation2ReservedBlock3 =
      new ReservedBlock(heap.ReserveMemory({.size = 1, .alignment = 1}));

  delete allocation1ReservedBlock1;
  delete allocation2ReservedBlock1;
  delete allocation1ReservedBlock2;
  delete allocation2ReservedBlock3;
  delete allocation2ReservedBlock2;
  delete allocation1ReservedBlock3;

  const ReservedBlock reservedBlock =
      heap.ReserveMemory({.size = 3, .alignment = 1});

  // Assert
  ASSERT_EQ(0, reservedBlock.GetMemoryBlock().offset);
  ASSERT_EQ(3, reservedBlock.GetMemoryBlock().size);
  ASSERT_EQ(2, allocator.allocations.size());
}

TEST(DeviceMemoryAllocatorTest,
     SimpleAllocationReturnsCorrectAllocationReference) {
  MemoryObjectWithId* const allocation1 = new MemoryObjectWithId(1);
  MemoryObjectWithId* const allocation2 = new MemoryObjectWithId(2);
  std::queue<MemoryObject*> objectsToAllocate;
  objectsToAllocate.emplace(allocation1);
  objectsToAllocate.emplace(allocation2);
  TestAllocator allocator(objectsToAllocate);
  DeviceHeap heap(1, &allocator);

  const ReservedBlock reservedBlock1 =
      heap.ReserveMemory({.size = 1, .alignment = 1});
  const ReservedBlock reservedBlock2 =
      heap.ReserveMemory({.size = 1, .alignment = 1});

  ASSERT_EQ(1, reservedBlock1.GetMemoryObjectAs<MemoryObjectWithId>().id);
  ASSERT_EQ(2, reservedBlock2.GetMemoryObjectAs<MemoryObjectWithId>().id);
}

TEST(DeviceMemoryAllocatorTest,
     MultipleReallocationsReturnCorrectMemoryObjectReference) {
  MemoryObjectWithId* const allocation1 = new MemoryObjectWithId(1);
  MemoryObjectWithId* const allocation2 = new MemoryObjectWithId(2);
  MemoryObjectWithId* const allocation3 = new MemoryObjectWithId(3);
  std::queue<MemoryObject*> objectsToAllocate;
  objectsToAllocate.emplace(allocation1);
  objectsToAllocate.emplace(allocation2);
  objectsToAllocate.emplace(allocation3);
  TestAllocator allocator(objectsToAllocate);
  DeviceHeap heap(1, &allocator);

  {
    const ReservedBlock reservedBlock1 =
        heap.ReserveMemory({.size = 1, .alignment = 1});
    const ReservedBlock reservedBlock2 =
        heap.ReserveMemory({.size = 2, .alignment = 1});
    const ReservedBlock reservedBlock3 =
        heap.ReserveMemory({.size = 3, .alignment = 1});
  }
  const ReservedBlock reservedBlock1 =
      heap.ReserveMemory({.size = 1, .alignment = 1});
  const ReservedBlock reservedBlock2 =
      heap.ReserveMemory({.size = 2, .alignment = 1});
  const ReservedBlock reservedBlock3 =
      heap.ReserveMemory({.size = 3, .alignment = 1});

  ASSERT_EQ(1, reservedBlock1.GetMemoryObjectAs<MemoryObjectWithId>().id);
  ASSERT_EQ(2, reservedBlock2.GetMemoryObjectAs<MemoryObjectWithId>().id);
  ASSERT_EQ(3, reservedBlock3.GetMemoryObjectAs<MemoryObjectWithId>().id);
}

TEST(DeviceMemoryAllocatorTest, IncreasesAllocationSizeBy150Percent) {
  TestAllocator allocator;
  DeviceHeap heap(2, &allocator);

  const ReservedBlock reservedBlock1 =
      heap.ReserveMemory({.size = 2, .alignment = 1});
  const ReservedBlock reservedBlock2 =
      heap.ReserveMemory({.size = 3, .alignment = 1});
  const ReservedBlock reservedBlock3 =
      heap.ReserveMemory({.size = 5, .alignment = 1});

  ASSERT_EQ(3, allocator.allocations.size());
  ASSERT_EQ(0, reservedBlock1.GetMemoryBlock().offset);
  ASSERT_EQ(2, reservedBlock1.GetMemoryBlock().size);
  ASSERT_EQ(0, reservedBlock2.GetMemoryBlock().offset);
  ASSERT_EQ(3, reservedBlock2.GetMemoryBlock().size);
  ASSERT_EQ(0, reservedBlock3.GetMemoryBlock().offset);
  ASSERT_EQ(5, reservedBlock3.GetMemoryBlock().size);
}

TEST(DeviceMemoryAllocatorTest, EnlargesHeapHeavilyToAccomodateLargeObject) {
  TestAllocator allocator;
  DeviceHeap heap(/* initialAllocationSize= */ 4, /* enlargementFactor= */ 2.0f,
                  &allocator);

  const ReservedBlock reservedBlock =
      heap.ReserveMemory({.size = 768, .alignment = 1});
  const ReservedBlock reservedBlock1 =
      heap.ReserveMemory({.size = 512, .alignment = 1});
  const ReservedBlock reservedBlock2 =
      heap.ReserveMemory({.size = ONE_MEGABYTE, .alignment = 1});
  const ReservedBlock reservedBlock3 =
      heap.ReserveMemory({.size = 2048, .alignment = 1});

  ASSERT_EQ(4, allocator.allocations.size());
  ASSERT_EQ(1024, allocator.allocations[0].size);
  ASSERT_EQ(2048, allocator.allocations[1].size);
  ASSERT_EQ(ONE_MEGABYTE, allocator.allocations[2].size);
  ASSERT_EQ(TWO_MEGABYTES, allocator.allocations[3].size);
}

TEST(DeviceMemoryAllocatorTest, AlignsAllocations) {
  TestAllocator allocator;
  DeviceHeap heap(1024, &allocator);

  const ReservedBlock forcedUnalignmentBlock =
      heap.ReserveMemory({.size = 513, .alignment = 1});
  const ReservedBlock alignedBlock =
      heap.ReserveMemory({.size = 8, .alignment = 256});

  ASSERT_EQ(768, alignedBlock.GetMemoryBlock().offset);
  ASSERT_EQ(8, alignedBlock.GetMemoryBlock().size);
}

TEST(DeviceMemoryAllocatorTest, GivesUpAlignmentFragmentsSmallerThan128Bytes) {
  TestAllocator allocator;
  DeviceHeap heap(1024, &allocator);

  const ReservedBlock forcedUnalignmentBlock =
      heap.ReserveMemory({.size = 1, .alignment = 1});
  const ReservedBlock alignedBlock =
      heap.ReserveMemory({.size = 1, .alignment = 128});
  const ReservedBlock smallBlockAllocatableBeforeAlignedBlock =
      heap.ReserveMemory({.size = 1, .alignment = 1});

  ASSERT_EQ(129,
            smallBlockAllocatableBeforeAlignedBlock.GetMemoryBlock().offset);
  ASSERT_EQ(1, smallBlockAllocatableBeforeAlignedBlock.GetMemoryBlock().size);
}

TEST(DeviceMemoryAllocatorTest, RetainsAlignmentFragmentsBiggerThan127Bytes) {
  TestAllocator allocator;
  DeviceHeap heap(1024, &allocator);

  const ReservedBlock forcedUnalignmentBlock =
      heap.ReserveMemory({.size = 1, .alignment = 1});
  const ReservedBlock alignedBlock =
      heap.ReserveMemory({.size = 8, .alignment = 129});
  const ReservedBlock smallBlockAllocatableBeforeAlignedBlock =
      heap.ReserveMemory({.size = 1, .alignment = 1});

  ASSERT_EQ(1, smallBlockAllocatableBeforeAlignedBlock.GetMemoryBlock().offset);
  ASSERT_EQ(1, smallBlockAllocatableBeforeAlignedBlock.GetMemoryBlock().size);
}

TEST(DeviceMemoryAllocatorTest, AlignedAllocationsReturnedCorrectly) {
  TestAllocator allocator;
  DeviceHeap heap(1024, &allocator);

  {
    const ReservedBlock reservedBlock1 =
        heap.ReserveMemory({.size = 1, .alignment = 1});
    const ReservedBlock reservedBlock2 =
        heap.ReserveMemory({.size = 1, .alignment = 5});
    const ReservedBlock reservedBlock3 =
        heap.ReserveMemory({.size = 1, .alignment = 10});
    const ReservedBlock reservedBlock4 =
        heap.ReserveMemory({.size = 1, .alignment = 128});
    const ReservedBlock reservedBlock5 =
        heap.ReserveMemory({.size = 1, .alignment = 512});
    const ReservedBlock reservedBlock6 =
        heap.ReserveMemory({.size = 256, .alignment = 768});
  }
  const ReservedBlock reservedBlock =
      heap.ReserveMemory({.size = 1024, .alignment = 1});

  ASSERT_EQ(0, reservedBlock.GetMemoryBlock().offset);
  ASSERT_EQ(1024, reservedBlock.GetMemoryBlock().size);
  ASSERT_EQ(1, allocator.allocations.size());
}

TEST(DeviceMemoryAllocatorTest, ReservedBlockCanBeMoveConstructed) {
  MemoryObjectWithId* const allocation = new MemoryObjectWithId();
  std::queue<MemoryObject*> objectsToAllocate;
  objectsToAllocate.emplace(allocation);
  TestAllocator allocator(objectsToAllocate);
  DeviceHeap heap(1, &allocator);

  const ReservedBlock reservedBlock(
      std::move(heap.ReserveMemory({.size = 1, .alignment = 1})));

  ASSERT_EQ(allocation, &reservedBlock.GetMemoryObjectAs<MemoryObjectWithId>());
}

TEST(DeviceMemoryAllocatorTest, ReservedBlockCanBeMoveAssigned) {
  MemoryObjectWithId* const allocation = new MemoryObjectWithId();
  std::queue<MemoryObject*> objectsToAllocate;
  objectsToAllocate.emplace(allocation);
  TestAllocator allocator(objectsToAllocate);
  DeviceHeap heap(1, &allocator);

  ReservedBlock reservedBlock;
  reservedBlock = heap.ReserveMemory({.size = 1, .alignment = 1});

  ASSERT_EQ(allocation, &reservedBlock.GetMemoryObjectAs<MemoryObjectWithId>());
}
