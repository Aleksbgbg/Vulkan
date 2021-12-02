#ifndef VULKAN_SRC_GENERAL_FILES_DRAWVERTICES_H_
#define VULKAN_SRC_GENERAL_FILES_DRAWVERTICES_H_

#include <vector>

#include "util/types.h"

template <typename TVertex>
struct DrawVertices {
  std::vector<TVertex> vertices;
  std::vector<u16> indices;
};

#endif  // VULKAN_SRC_GENERAL_FILES_DRAWVERTICES_H_
