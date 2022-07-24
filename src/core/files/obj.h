#ifndef SRC_CORE_FILES_OBJ_H_
#define SRC_CORE_FILES_OBJ_H_

#include <array>
#include <istream>
#include <string_view>
#include <vector>

#include "asset.h"
#include "core/types.h"

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
  bool operator!=(const ModelFaceVertex other) const;

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

Model ModelFromAsset(asset::Model model);
Model ModelFromString(const std::string& string);

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

#endif  // SRC_CORE_FILES_OBJ_H_
