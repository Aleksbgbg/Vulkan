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
      parts.emplace_back(string.substr(lastMatch, string.size() - lastMatch));
      break;
    } else {
      parts.emplace_back(string.substr(lastMatch, newMatch - lastMatch));
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
          model.normalVertices.emplace_back(
              ModelNormalVertex{.x = std::stof(parts[1]),
                                .y = std::stof(parts[2]),
                                .z = std::stof(parts[3])});
        } else if (line.starts_with("v ")) {
          model.vertices.emplace_back(ModelVertex{.x = std::stof(parts[1]),
                                                  .y = std::stof(parts[2]),
                                                  .z = std::stof(parts[3])});
        } else if (line.starts_with("vt ")) {
          model.textureVertices.emplace_back(ModelTextureVertex{
              .u = std::stof(parts[1]), .v = std::stof(parts[2])});
        } else if (line.starts_with("f ")) {
          ModelFace modelFace;

          for (u32 index = 1; index < parts.size(); ++index) {
            const std::vector<std::string> components =
                Split(parts[index], "/");

            modelFace.faceVertices[index - 1] = ModelFaceVertex{
                .vertexIndex = static_cast<u32>(std::stoi(components[0])) - 1,
                .normalVertexIndex =
                    static_cast<u32>(std::stoi(components[2])) - 1,
                .textureVertexIndex =
                    static_cast<u32>(std::stoi(components[1])) - 1,
            };
          }

          model.faces.emplace_back(modelFace);
        }
      }

      line.clear();
    } else {
      line.push_back(nextChar);
    }
  }

  return model;
}

}  // namespace file
