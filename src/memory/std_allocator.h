#ifndef VULKAN_SRC_MEMORY_STD_ALLOCATOR_H_
#define VULKAN_SRC_MEMORY_STD_ALLOCATOR_H_

#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <new>
#include <unordered_map>
#include <vector>

#include "Allocator.h"
#include "DeviceHeap.h"

class Alloc : public Allocator {
 public:
  struct Mem : public MemoryObject {
    Mem(u64 size) {
      mem = malloc(size);
    }

    Mem(Mem&) = delete;
    Mem(Mem&& other) noexcept {
      mem = other.mem;
      other.mem = nullptr;
    }

    ~Mem() {
      if (mem != nullptr) {
        free(mem);
        mem = nullptr;
      }
    }

    Mem& operator=(Mem&) = delete;
    Mem& operator=(Mem&& other) noexcept {
      std::swap(mem, other.mem);
      return *this;
    }

    void* mem;
  };

  u64 MemoryObjectSize() const override {
    return sizeof(Mem);
  }

  MemoryObject* Allocate(void* memoryObjectMemory, u64 size) override {
    return new (memoryObjectMemory) Mem(size);
  }
};

DeviceHeap& Heap();

template <class T>
struct Mallocator {
  typedef T value_type;

  Mallocator() : Mallocator(Heap()) {}
  Mallocator(DeviceHeap& heap)
      : heap_(&heap), blocks_(new std::unordered_map<T*, ReservedBlock>) {}
  template <class U>
  Mallocator(const Mallocator<U>& a) noexcept : Mallocator(*a.heap_) {}

  [[nodiscard]] T* allocate(std::size_t n) {
    if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
      throw std::bad_array_new_length();

    auto b = heap_->ReserveMemory({.size = n, .alignment = 1});
    auto& c = b.GetMemoryObjectAs<Alloc::Mem>();
    T* p = reinterpret_cast<T*>(reinterpret_cast<u8*>(c.mem) +
                                b.GetMemoryBlock().offset);
    blocks_->insert(std::make_pair(p, std::move(b)));

    report(p, n);
    return p;

    throw std::bad_alloc();
  }

  void deallocate(T* p, std::size_t n) noexcept {
    report(p, n, 0);
    blocks_->erase(blocks_->find(p));
  }

  DeviceHeap* heap_;

 private:
  void report(T* p, std::size_t n, bool alloc = true) const {
    std::cout << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(T) * n
              << " bytes at " << std::hex << std::showbase
              << reinterpret_cast<void*>(p) << std::dec << std::endl;
  }
  std::unordered_map<T*, ReservedBlock>* blocks_;
};

template <class T, class U>
bool operator==(const Mallocator<T>&, const Mallocator<U>&) {
  return true;
}
template <class T, class U>
bool operator!=(const Mallocator<T>&, const Mallocator<U>&) {
  return false;
}

DeviceHeap& Heap() {
  static Alloc all;
  static DeviceHeap heap(100u * 1024u * 1024u, &all);
  return heap;
}

template <typename T>
Mallocator<T> GetAllocator() {
  return Mallocator<T>(Heap());
}

#endif  // VULKAN_SRC_MEMORY_STD_ALLOCATOR_H_
