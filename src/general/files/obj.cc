#include "obj.h"

#include <fstream>
#include <string>

namespace file {

std::vector<std::string> Split(const std::string& string,
                               const std::string_view delimiter) {
  std::vector<std::string> parts;

  size_t lastMatch = 0;

  while (true) {
    size_t newMatch = string.find(delimiter, lastMatch);

    if (newMatch == std::string::npos) {
      parts.push_back(string.substr(lastMatch, string.size() - lastMatch));
      break;
    } else {
      parts.push_back(string.substr(lastMatch, newMatch - lastMatch));
    }

    lastMatch = newMatch + delimiter.size();
  }

  return parts;
}

Model ModelFromObjFile(const std::string_view name) {
  std::ifstream file(name.data());

  if (!file.is_open()) {
    throw new std::runtime_error(std::string("Could not open obj file ") +
                                 name.data());
  }

  std::string line;
  Model model;

  while (!file.eof()) {
    const auto nextChar = file.get();

    if (nextChar == '\n') {
      if (line[0] != '#') {
        const std::vector<std::string> parts = Split(line, " ");

        if (line.starts_with("vn ")) {
          model.normalVertices.push_back(
              ModelNormalVertex{.x = std::stof(parts[1]),
                                .y = std::stof(parts[2]),
                                .z = std::stof(parts[3])});
        } else if (line.starts_with("v ")) {
          model.vertices.push_back(ModelVertex{.x = std::stof(parts[1]),
                                               .y = std::stof(parts[2]),
                                               .z = std::stof(parts[3])});
        } else if (line.starts_with("vt ")) {
          model.textureVertices.push_back(ModelTextureVertex{
              .u = std::stof(parts[1]), .v = std::stof(parts[2])});
        } else if (line.starts_with("f ")) {
          ModelFace modelFace;

          if (parts.size() <= 4) {
            for (u32 index = 1; index < parts.size(); ++index) {
              const std::vector<std::string> components =
                  Split(parts[index], "/");

              const u32 vertexIndex =
                  static_cast<u32>(std::stoi(components[0])) - 1;
              const u32 normalVertexIndex =
                  static_cast<u32>(std::stoi(components[2])) - 1;
              const u32 textureVertexIndex =
                  components[1].size() == 0
                      ? 0
                      : static_cast<u32>(std::stoi(components[1])) - 1;

              modelFace.faceVertices[index - 1] = ModelFaceVertex{
                  .vertexIndex = vertexIndex,
                  .normalVertexIndex = normalVertexIndex,
                  .textureVertexIndex = textureVertexIndex,
              };
            }

            model.faces.push_back(modelFace);
          } else {
            for (u32 index = 1; index <= 3; ++index) {
              const std::vector<std::string> components =
                  Split(parts[index], "/");

              const u32 vertexIndex =
                  static_cast<u32>(std::stoi(components[0])) - 1;
              const u32 normalVertexIndex =
                  static_cast<u32>(std::stoi(components[2])) - 1;
              const u32 textureVertexIndex =
                  components[1].size() == 0
                      ? 0
                      : static_cast<u32>(std::stoi(components[1])) - 1;

              modelFace.faceVertices[index - 1] = ModelFaceVertex{
                  .vertexIndex = vertexIndex,
                  .normalVertexIndex = normalVertexIndex,
                  .textureVertexIndex = textureVertexIndex,
              };
            }

            model.faces.push_back(modelFace);

            u32 i = 0;
            u32 j = 0;

            for (; i <= 3;) {
              if (i == 1) {
                ++i;
                continue;
              }

              const std::vector<std::string> components =
                  Split(parts[i + 1], "/");

              const u32 vertexIndex =
                  static_cast<u32>(std::stoi(components[0])) - 1;
              const u32 normalVertexIndex =
                  static_cast<u32>(std::stoi(components[2])) - 1;
              const u32 textureVertexIndex =
                  components[1].size() == 0
                      ? 0
                      : static_cast<u32>(std::stoi(components[1])) - 1;

              modelFace.faceVertices[j] =ModelFaceVertex{
                  .vertexIndex = vertexIndex,
                  .normalVertexIndex = normalVertexIndex,
                  .textureVertexIndex = textureVertexIndex,
              };

              ++i;
              ++j;
            }

            model.faces.push_back(modelFace);
          }
        }
      }

      line.clear();
    } else {
      line.push_back(nextChar);
    }
  }

  if (model.textureVertices.size() == 0) {
    model.textureVertices.push_back(ModelTextureVertex{.u = 0, .v = 0});
  }

  return model;
}

}  // namespace file
