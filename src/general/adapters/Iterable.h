#ifndef VULKAN_SRC_GENERAL_ADAPTERS_ITERABLE_H_
#define VULKAN_SRC_GENERAL_ADAPTERS_ITERABLE_H_

#include <iterator>

template <typename TIterator>
class Iterable {
 public:
  constexpr Iterable(const TIterator begin, const TIterator end)
      : begin_(begin), end_(end) {}

  constexpr TIterator begin() const {
    return begin_;
  }

  constexpr TIterator end() const {
    return end_;
  }

 private:
  TIterator begin_;
  TIterator end_;
};

template <typename TContainer>
auto IterateAll(TContainer& container) {
  return Iterable(std::begin(container), std::end(container));
}

#endif  // VULKAN_SRC_GENERAL_ADAPTERS_ITERABLE_H_
