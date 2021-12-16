#ifndef VULKAN_SRC_GENERAL_FILES_OBJ_H_
#define VULKAN_SRC_GENERAL_FILES_OBJ_H_

#include <array>
#include <istream>
#include <string_view>
#include <vector>

#include "util/types.h"

namespace file {

struct ModelVertex {
  float x;
  float y;
  float z;
};

struct ModelNormalVertex {
  float x;
  float y;
  float z;
};

struct ModelTextureVertex {
  float u;
  float v;
};

struct ModelFaceVertex {
  bool operator==(const ModelFaceVertex other) const;

  u32 vertexIndex;
  u32 normalVertexIndex;
  u32 textureVertexIndex;
};

struct ModelFace {
  bool operator==(const ModelFace& other) const;

  std::array<ModelFaceVertex, 3> faceVertices;
};

struct Model {
  std::vector<ModelVertex> vertices;
  std::vector<ModelNormalVertex> normalVertices;
  std::vector<ModelTextureVertex> textureVertices;

  std::vector<ModelFace> faces;
};

Model ModelFromObjFile(const std::string_view name);

Model ModelFromStream(std::istream& stream);

}  // namespace file

namespace std {

template <>
class hash<file::ModelFaceVertex> {
 public:
  size_t operator()(const file::ModelFaceVertex faceVertex) const;
};

template <>
class hash<file::ModelFace> {
 public:
  size_t operator()(const file::ModelFace& face) const;
};

}  // namespace std

#endif  // VULKAN_SRC_GENERAL_FILES_OBJ_H_
