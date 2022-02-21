#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <sstream>

#include "core/files/obj.h"

namespace {

using testing::ElementsAre;
using testing::Eq;
using testing::IsEmpty;
using testing::SizeIs;

}  // namespace

namespace file {

std::ostream& operator<<(std::ostream& os,
                         const file::ModelFaceVertex& modelFaceVertex) {
  return os << "file::ModelFaceVertex{ vertexIndex = "
            << modelFaceVertex.vertexIndex
            << ", normalVertexIndex = " << modelFaceVertex.normalVertexIndex
            << ", textureVertexIndex = " << modelFaceVertex.textureVertexIndex
            << " }";
}

}  // namespace file

TEST(ReadObjTest, ReadsEmptyFile) {
  std::istringstream stream;

  const file::Model model = file::ModelFromStream(stream);

  EXPECT_THAT(model.vertices, IsEmpty());
  EXPECT_THAT(model.normalVertices, IsEmpty());
  EXPECT_THAT(model.textureVertices, IsEmpty());
  EXPECT_THAT(model.faces, IsEmpty());
}

TEST(ReadObjTest, IgnoresUninterestingAttributes) {
  std::istringstream stream(
      "# Blender OBJ File\n"
      "mtllib Sphere.mtl\n"
      "o Sphere\n");

  const file::Model model = file::ModelFromStream(stream);

  EXPECT_THAT(model.vertices, IsEmpty());
  EXPECT_THAT(model.normalVertices, IsEmpty());
  EXPECT_THAT(model.textureVertices, IsEmpty());
  EXPECT_THAT(model.faces, IsEmpty());
}

TEST(ReadObjTest, ParsesVertices) {
  std::istringstream stream(
      "v 1 2 3\n"
      "v 4 5 6\n");

  const file::Model model = file::ModelFromStream(stream);

  ASSERT_THAT(model.vertices, SizeIs(2));
  EXPECT_EQ(model.vertices[0].x, 1.0f);
  EXPECT_EQ(model.vertices[0].y, 2.0f);
  EXPECT_EQ(model.vertices[0].z, 3.0f);
  EXPECT_EQ(model.vertices[1].x, 4.0f);
  EXPECT_EQ(model.vertices[1].y, 5.0f);
  EXPECT_EQ(model.vertices[1].z, 6.0f);
}

TEST(ReadObjTest, ParsesFloatingPointVertex) {
  std::istringstream stream("v -0.0 0.0 -1.2345");

  const file::Model model = file::ModelFromStream(stream);

  ASSERT_THAT(model.vertices, SizeIs(1));
  EXPECT_EQ(model.vertices[0].x, -0.0f);
  EXPECT_EQ(model.vertices[0].y, 0.0f);
  EXPECT_EQ(model.vertices[0].z, -1.2345f);
}

TEST(ReadObjTest, ParsesNormalVertices) {
  std::istringstream stream(
      "vn 1 2 3\n"
      "vn 4 5 6\n");

  const file::Model model = file::ModelFromStream(stream);

  ASSERT_THAT(model.normalVertices, SizeIs(2));
  EXPECT_EQ(model.normalVertices[0].x, 1.0f);
  EXPECT_EQ(model.normalVertices[0].y, 2.0f);
  EXPECT_EQ(model.normalVertices[0].z, 3.0f);
  EXPECT_EQ(model.normalVertices[1].x, 4.0f);
  EXPECT_EQ(model.normalVertices[1].y, 5.0f);
  EXPECT_EQ(model.normalVertices[1].z, 6.0f);
}

TEST(ReadObjTest, ParsesTextureVertices) {
  std::istringstream stream(
      "vt 1 2\n"
      "vt 3 4\n");

  const file::Model model = file::ModelFromStream(stream);

  ASSERT_THAT(model.textureVertices, SizeIs(2));
  EXPECT_EQ(model.textureVertices[0].u, 1.0f);
  EXPECT_EQ(model.textureVertices[0].v, 2.0f);
  EXPECT_EQ(model.textureVertices[1].u, 3.0f);
  EXPECT_EQ(model.textureVertices[1].v, 4.0f);
}

TEST(ReadObjTest, ParsesVertexOnlyFace) {
  std::istringstream stream("f 1// 2// 3//");

  const file::Model model = file::ModelFromStream(stream);

  ASSERT_THAT(model.faces, SizeIs(1));
  EXPECT_THAT(model.faces[0].faceVertices,
              ElementsAre(file::ModelFaceVertex{.vertexIndex = 0,
                                                .normalVertexIndex = 0,
                                                .textureVertexIndex = 0},
                          file::ModelFaceVertex{.vertexIndex = 1,
                                                .normalVertexIndex = 0,
                                                .textureVertexIndex = 0},
                          file::ModelFaceVertex{.vertexIndex = 2,
                                                .normalVertexIndex = 0,
                                                .textureVertexIndex = 0}));
}

TEST(ReadObjTest, ParsesVertexAndNormalFace) {
  std::istringstream stream("f 1//1 2//2 3//3");

  const file::Model model = file::ModelFromStream(stream);

  ASSERT_THAT(model.faces, SizeIs(1));
  EXPECT_THAT(model.faces[0].faceVertices,
              ElementsAre(file::ModelFaceVertex{.vertexIndex = 0,
                                                .normalVertexIndex = 0,
                                                .textureVertexIndex = 0},
                          file::ModelFaceVertex{.vertexIndex = 1,
                                                .normalVertexIndex = 1,
                                                .textureVertexIndex = 0},
                          file::ModelFaceVertex{.vertexIndex = 2,
                                                .normalVertexIndex = 2,
                                                .textureVertexIndex = 0}));
}

TEST(ReadObjTest, ParsesVertexTextureNormalFace) {
  std::istringstream stream("f 1/1/1 2/2/2 3/3/3");

  const file::Model model = file::ModelFromStream(stream);

  ASSERT_THAT(model.faces, SizeIs(1));
  EXPECT_THAT(model.faces[0].faceVertices,
              ElementsAre(file::ModelFaceVertex{.vertexIndex = 0,
                                                .normalVertexIndex = 0,
                                                .textureVertexIndex = 0},
                          file::ModelFaceVertex{.vertexIndex = 1,
                                                .normalVertexIndex = 1,
                                                .textureVertexIndex = 1},
                          file::ModelFaceVertex{.vertexIndex = 2,
                                                .normalVertexIndex = 2,
                                                .textureVertexIndex = 2}));
}

TEST(ReadObjTest, ParsesMultipleFaces) {
  std::istringstream stream(
      "f 1/1/1 2/2/2 3/3/3\n"
      "f 6/6/6 5/5/5 4/4/4\n");

  const file::Model model = file::ModelFromStream(stream);

  ASSERT_THAT(model.faces, SizeIs(2));
  EXPECT_THAT(model.faces[0].faceVertices,
              ElementsAre(file::ModelFaceVertex{.vertexIndex = 0,
                                                .normalVertexIndex = 0,
                                                .textureVertexIndex = 0},
                          file::ModelFaceVertex{.vertexIndex = 1,
                                                .normalVertexIndex = 1,
                                                .textureVertexIndex = 1},
                          file::ModelFaceVertex{.vertexIndex = 2,
                                                .normalVertexIndex = 2,
                                                .textureVertexIndex = 2}));
  EXPECT_THAT(model.faces[1].faceVertices,
              ElementsAre(file::ModelFaceVertex{.vertexIndex = 5,
                                                .normalVertexIndex = 5,
                                                .textureVertexIndex = 5},
                          file::ModelFaceVertex{.vertexIndex = 4,
                                                .normalVertexIndex = 4,
                                                .textureVertexIndex = 4},
                          file::ModelFaceVertex{.vertexIndex = 3,
                                                .normalVertexIndex = 3,
                                                .textureVertexIndex = 3}));
}

TEST(ReadObjTest, ParsesSquareFace) {
  std::istringstream stream("f 1/1/1 2/2/2 3/3/3 4/4/4");

  const file::Model model = file::ModelFromStream(stream);

  ASSERT_THAT(model.faces, SizeIs(2));
  EXPECT_THAT(model.faces[0].faceVertices,
              ElementsAre(file::ModelFaceVertex{.vertexIndex = 0,
                                                .normalVertexIndex = 0,
                                                .textureVertexIndex = 0},
                          file::ModelFaceVertex{.vertexIndex = 1,
                                                .normalVertexIndex = 1,
                                                .textureVertexIndex = 1},
                          file::ModelFaceVertex{.vertexIndex = 2,
                                                .normalVertexIndex = 2,
                                                .textureVertexIndex = 2}));
  EXPECT_THAT(model.faces[1].faceVertices,
              ElementsAre(file::ModelFaceVertex{.vertexIndex = 0,
                                                .normalVertexIndex = 0,
                                                .textureVertexIndex = 0},
                          file::ModelFaceVertex{.vertexIndex = 2,
                                                .normalVertexIndex = 2,
                                                .textureVertexIndex = 2},
                          file::ModelFaceVertex{.vertexIndex = 3,
                                                .normalVertexIndex = 3,
                                                .textureVertexIndex = 3}));
}
