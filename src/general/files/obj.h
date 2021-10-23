#ifndef VULKAN_SRC_MODEL_H
#define VULKAN_SRC_MODEL_H

#include <array>
#include <vector>

#include "util/types.h"

namespace file {

struct ModelVertex {
  float x;
  float y;
  float z;
};

struct ModelTextureVertex {
  float u;
  float v;
};

struct ModelFaceVertex {
  bool operator==(const ModelFaceVertex other) const {
    return (vertexIndex == other.vertexIndex) &&
           (textureVertexIndex == other.textureVertexIndex);
  }

  u32 vertexIndex;
  u32 textureVertexIndex;
};

struct ModelFace {
  bool operator==(const ModelFace& other) const {
    for (u32 index = 0; index < faceVertices.size(); ++index) {
      if (faceVertices[index] != other.faceVertices[index]) {
        return false;
      }
    }

    return true;
  }

  std::array<ModelFaceVertex, 3> faceVertices;
};

struct Model {
  std::vector<ModelVertex> vertices;
  std::vector<ModelTextureVertex> textureVertices;
  std::vector<ModelFace> faces;
};

Model ModelFromObjFile(const char* const name);

}  // namespace file

namespace std {

template <>
class hash<file::ModelFaceVertex> {
 public:
  size_t operator()(const file::ModelFaceVertex faceVertex) const {
    return std::hash<u32>()(faceVertex.vertexIndex) ^
           std::hash<u32>()(faceVertex.textureVertexIndex);
  }
};

template <>
class hash<file::ModelFace> {
 public:
  size_t operator()(const file::ModelFace& face) const {
    size_t hash = 0;

    for (u32 index = 0; index < face.faceVertices.size(); ++index) {
      hash ^= std::hash<file::ModelFaceVertex>()(face.faceVertices[index]);
    }

    return hash;
  }
};

}  // namespace std

#endif  // VULKAN_SRC_MODEL_H
