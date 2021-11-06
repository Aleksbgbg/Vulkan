#ifndef VULKAN_SRC_GENERAL_FILES_DRAWVERTICES_H
#define VULKAN_SRC_GENERAL_FILES_DRAWVERTICES_H

#include <vector>

#include "util/types.h"

template <typename TVertex>
struct DrawVertices {
  std::vector<TVertex> vertices;
  std::vector<u16> indices;
};

#endif  // VULKAN_SRC_GENERAL_FILES_DRAWVERTICES_H
