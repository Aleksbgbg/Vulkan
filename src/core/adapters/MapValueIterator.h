#ifndef VULKAN_SRC_CORE_ADAPTERS_MAPVALUEITERATOR_H_
#define VULKAN_SRC_CORE_ADAPTERS_MAPVALUEITERATOR_H_

#include <type_traits>

template <typename TMap>
class MapValueIterable {
 public:
  class Iterator {
    using ValueType = typename TMap::mapped_type;
    using IteratorType =
        std::conditional_t<std::is_const_v<TMap>, typename TMap::const_iterator,
                           typename TMap::iterator>;
    using ValueTypeWithConstness =
        std::conditional_t<std::is_const_v<TMap>, std::add_const_t<ValueType>,
                           ValueType>;

   public:
    explicit Iterator(IteratorType iterator) : iterator_(iterator) {}

    bool operator!=(const Iterator& other) const {
      return iterator_ != other.iterator_;
    }

    Iterator& operator++() {
      ++iterator_;
      return *this;
    }

    ValueTypeWithConstness& operator*() const {
      return iterator_->second;
    }

   private:
    IteratorType iterator_;
  };

  explicit MapValueIterable(TMap& map) : map_(&map) {}

  Iterator begin() const {
    return Iterator(map_->begin());
  }

  Iterator end() const {
    return Iterator(map_->end());
  }

 private:
  TMap* map_;
};

template <typename TMap>
MapValueIterable<TMap> IterateValues(TMap& map) {
  return MapValueIterable(map);
}

template <typename TMap>
auto& FirstValue(TMap& map) {
  return map.begin()->second;
}

#endif  // VULKAN_SRC_CORE_ADAPTERS_MAPVALUEITERATOR_H_
