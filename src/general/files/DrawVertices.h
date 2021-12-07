#ifndef VULKAN_SRC_GENERAL_FILES_DRAWVERTICES_H_
#define VULKAN_SRC_GENERAL_FILES_DRAWVERTICES_H_

#include <vector>

#include "util/types.h"

template <typename TVertex>
struct DrawVertices {
  std::vector<TVertex> vertices;
  std::vector<u16> indices;

  std::size_t VerticesMemorySize() const {
    return vertices.size() * sizeof(TVertex);
  }

  std::size_t IndicesMemorySize() const {
    return indices.size() * sizeof(u16);
  }
};

#endif  // VULKAN_SRC_GENERAL_FILES_DRAWVERTICES_H_
