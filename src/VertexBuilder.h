#ifndef VULKAN_SRC_VERTEXBUILDER_H
#define VULKAN_SRC_VERTEXBUILDER_H

#include <vector>

#include "types.h"
#include "Vertex.h"

typedef u16 Index;

class VertexBuilder {
public:
  explicit VertexBuilder(const std::vector<Vertex>& vertices);
  std::vector<Index> GenerateIndices();

private:
  struct VertexInfo {
    glm::vec3 pos;
    bool visited;
  };

  glm::vec3 Position(const Index index) const;

  std::vector<Index> FindNeighbours(const Index index) const;

private:
  std::vector<VertexInfo> vertices;
};

#endif // VULKAN_SRC_VERTEXBUILDER_H
