#ifndef VULKAN_SRC_MODEL_H
#define VULKAN_SRC_MODEL_H

#include <array>
#include <vector>

#include "util/types.h"

namespace obj {

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
  u32 vertexIndex;
  u32 textureVertexIndex;
};

struct ModelFace {
  std::array<ModelFaceVertex, 3> faceVertices;
};

struct Model {
  std::vector<ModelVertex> vertices;
  std::vector<ModelTextureVertex> textureVertices;
  std::vector<ModelFace> faces;
};

Model ModelFromObjFile(const char* const name);

}  // namespace obj

#endif  // VULKAN_SRC_MODEL_H
