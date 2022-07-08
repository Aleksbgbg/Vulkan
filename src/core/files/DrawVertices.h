#ifndef SRC_CORE_FILES_DRAWVERTICES_H_
#define SRC_CORE_FILES_DRAWVERTICES_H_

#include <vector>

#include "core/types.h"

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

#endif  // SRC_CORE_FILES_DRAWVERTICES_H_
