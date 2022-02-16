#ifndef VULKAN_SRC_RENDERER_VERTICES_STRUCTUREDVERTEXDATA_H_
#define VULKAN_SRC_RENDERER_VERTICES_STRUCTUREDVERTEXDATA_H_

#include "general/files/DrawVertices.h"
#include "util/include/glm.h"
#include "util/types.h"

class StructuredVertexData {
 public:
  virtual ~StructuredVertexData() = default;

  virtual void Apply(const glm::mat4& transformation) = 0;

  struct RawVertexData {
    const void* vertices;
    std::size_t verticesMemorySize;
    const void* indices;
    std::size_t indicesMemorySize;
    u32 indexCount;
  };
  virtual RawVertexData GetRawVertexData() const = 0;
};

template <typename T>
class VertexDataOfType : public StructuredVertexData {
 public:
  VertexDataOfType(DrawVertices<T> indexedVertices)
      : indexedVertices_(std::move(indexedVertices)) {}

  void Apply(const glm::mat4& transformation) override {
    for (T& vertex : indexedVertices_.vertices) {
      vertex.position = glm::vec4(vertex.position, 1.0f) * transformation;
    }
  }

  RawVertexData GetRawVertexData() const override {
    return {.vertices = indexedVertices_.vertices.data(),
            .verticesMemorySize = indexedVertices_.VerticesMemorySize(),
            .indices = indexedVertices_.indices.data(),
            .indicesMemorySize = indexedVertices_.IndicesMemorySize(),
            .indexCount = static_cast<u32>(indexedVertices_.indices.size())};
  }

 private:
  DrawVertices<T> indexedVertices_;
};

#endif  // VULKAN_SRC_RENDERER_VERTICES_STRUCTUREDVERTEXDATA_H_
