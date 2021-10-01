#include "VertexBuilder.h"

#include <algorithm>
#include <array>
#include <iterator>

#include "Vertex.h"

VertexBuilder::VertexBuilder(const std::vector<Vertex>& vertices) : vertices(vertices.size()) {
  std::transform(
      vertices.begin(),
      vertices.end(),
      this->vertices.begin(),
      [](const Vertex& vertex) {
        return VertexInfo{
          .pos = vertex.position,
          .visited = false
        };
      });
}

std::vector<Index> VertexBuilder::GenerateIndices() {
  std::vector<Index> indices;

  if (vertices.size() < 3) {
    return indices;
  }

  for (Index index = 0; index < vertices.size(); ++index) {
    vertices[index].visited = true;

    // indices.push_back(index);

    std::vector<Index> neighbours = FindNeighbours(index);
    bool shouldGenerateMesh = true;

//    for (Index neighbour : neighbours) {
//      if (vertices[neighbour].visited) {
//        shouldGenerateMesh = false;
//        break;
//      }
//    }

    if (shouldGenerateMesh) {
      for (u32 neighbourIndex = 0; neighbourIndex < neighbours.size(); ++neighbourIndex) {
        Index neighbour = neighbours[neighbourIndex];

        indices.push_back(index);
        indices.push_back(neighbour);
        indices.push_back(neighbours[(neighbourIndex + 1) % neighbours.size()]);
      }
    }
  }

  return indices;
}

glm::vec3 VertexBuilder::Position(const Index index) const {
  return vertices[index].pos;
}

std::vector<Index> VertexBuilder::FindNeighbours(const Index index) const {
  glm::vec3 rootPosition = vertices[index].pos;

  struct NeighbourInfo {
    Index index;
    float distance;
  };
  std::vector<NeighbourInfo> neighbours;

  for (Index neighbour = 0; neighbour < vertices.size(); ++neighbour) {
    if (neighbour == index) {
      continue;
    }

    neighbours.emplace_back(NeighbourInfo{
      .index = neighbour,
      .distance = glm::distance(rootPosition, Position(neighbour))
    });
  }

  std::vector<Index> indices;
  std::sort(neighbours.begin(), neighbours.end(), [](const NeighbourInfo a, const NeighbourInfo b) {
    return a.distance < b.distance;
  });
  std::transform(neighbours.begin(), neighbours.begin() + 3, std::back_inserter(indices), [](const NeighbourInfo neighbourInfo) {
    return neighbourInfo.index;
  });
  return indices;
}
