#include "obj.h"

#include <fstream>
#include <string>

#include "general/strings/split.h"

namespace file {

float AsFloat(const std::string_view string) {
  return std::stof(string.data());
}

u32 AsUnsigned(const std::string_view string) {
  return std::stoul(string.data());
}

u32 AsIndex(const std::string_view string) {
  return AsUnsigned(string) - 1;
}

class LineHandler {
 public:
  virtual ~LineHandler() = default;

  virtual bool CanHandle(const std::string_view line) const = 0;
  virtual void Handle(const std::vector<std::string>& splitLineParts) = 0;
};

class VertexHandler : public LineHandler {
 public:
  VertexHandler(Model& model) : model_(model) {}

  bool CanHandle(const std::string_view line) const override {
    return line.starts_with("v ");
  }

  void Handle(const std::vector<std::string>& splitLineParts) override {
    const float x = AsFloat(splitLineParts[0]);
    const float y = AsFloat(splitLineParts[1]);
    const float z = AsFloat(splitLineParts[2]);
    model_.vertices.push_back({.x = x, .y = y, .z = z});
  }

 private:
  Model& model_;
};

class NormalHandler : public LineHandler {
 public:
  NormalHandler(Model& model) : model_(model) {}

  bool CanHandle(const std::string_view line) const override {
    return line.starts_with("vn ");
  }

  void Handle(const std::vector<std::string>& splitLineParts) override {
    const float x = AsFloat(splitLineParts[0]);
    const float y = AsFloat(splitLineParts[1]);
    const float z = AsFloat(splitLineParts[2]);
    model_.normalVertices.push_back({.x = x, .y = y, .z = z});
  }

 private:
  Model& model_;
};

class TextureHandler : public LineHandler {
 public:
  TextureHandler(Model& model) : model_(model) {}

  bool CanHandle(const std::string_view line) const override {
    return line.starts_with("vt ");
  }

  void Handle(const std::vector<std::string>& splitLineParts) override {
    const float u = AsFloat(splitLineParts[0]);
    const float v = AsFloat(splitLineParts[1]);
    model_.textureVertices.push_back({.u = u, .v = v});
  }

 private:
  Model& model_;
};

class FaceHandler : public LineHandler {
 public:
  FaceHandler(Model& model) : model_(model) {}

  bool CanHandle(const std::string_view line) const override {
    return line.starts_with("f ");
  }

  void Handle(const std::vector<std::string>& splitLineParts) override {
    if (splitLineParts.size() == 3) {
      HandleTriangularFace(splitLineParts);
    } else {
      HandleSquareFace(splitLineParts);
    }
  }

 private:
  void HandleTriangularFace(const std::vector<std::string>& splitLineParts) {
    ModelFace modelFace;
    for (u32 index = 0; index < splitLineParts.size(); ++index) {
      modelFace.faceVertices[index] = AsFace(Split(splitLineParts[index], "/"));
    }
    model_.faces.push_back(modelFace);
  }

  void HandleSquareFace(const std::vector<std::string>& splitLineParts) {
    const std::vector<std::string>& partsVertex1 =
        Split(splitLineParts[0], "/");
    const std::vector<std::string>& partsVertex2 =
        Split(splitLineParts[1], "/");
    const std::vector<std::string>& partsVertex3 =
        Split(splitLineParts[2], "/");
    const std::vector<std::string>& partsVertex4 =
        Split(splitLineParts[3], "/");

    std::array<ModelFaceVertex, 3> triangle1;
    triangle1[0] = AsFace(partsVertex1);
    triangle1[1] = AsFace(partsVertex2);
    triangle1[2] = AsFace(partsVertex3);
    model_.faces.push_back(ModelFace{.faceVertices = triangle1});

    std::array<ModelFaceVertex, 3> triangle2;
    triangle2[0] = AsFace(partsVertex1);
    triangle2[1] = AsFace(partsVertex3);
    triangle2[2] = AsFace(partsVertex4);
    model_.faces.push_back(ModelFace{.faceVertices = triangle2});
  }

  static ModelFaceVertex AsFace(const std::vector<std::string>& parts) {
    const u32 vertexIndex = AsIndex(parts[0]);
    const u32 normalVertexIndex = parts[2].empty() ? 0 : AsIndex(parts[2]);
    const u32 textureVertexIndex = parts[1].empty() ? 0 : AsIndex(parts[1]);
    return {
        .vertexIndex = vertexIndex,
        .normalVertexIndex = normalVertexIndex,
        .textureVertexIndex = textureVertexIndex,
    };
  }

 private:
  Model& model_;
};

Model ModelFromStream(std::istream& stream) {
  Model model;

  VertexHandler vertexHandler(model);
  NormalHandler normalHandler(model);
  TextureHandler textureHandler(model);
  FaceHandler faceHandler(model);

  std::array<LineHandler*, 4> lineHandlers{
      &vertexHandler,
      &normalHandler,
      &textureHandler,
      &faceHandler,
  };

  std::string line;

  while (true) {
    const auto nextChar = stream.get();

    if (nextChar == '\n' || stream.eof()) {
      for (LineHandler* handler : lineHandlers) {
        if (handler->CanHandle(line)) {
          const std::vector<std::string> lineParts =
              Split(line.substr(line.find(' ') + 1), " ");
          handler->Handle(lineParts);
          break;
        }
      }

      line.clear();
    } else {
      line.push_back(nextChar);
    }

    if (stream.eof()) {
      break;
    }
  }

  return model;
}

Model ModelFromObjFile(const std::string_view name) {
  std::ifstream file(name.data());

  if (!file.is_open()) {
    throw new std::runtime_error(std::string("Could not open obj file ") +
                                 name.data());
  }

  return ModelFromStream(file);
}

bool ModelFaceVertex::operator==(const ModelFaceVertex other) const {
  return (vertexIndex == other.vertexIndex) &&
         (normalVertexIndex == other.normalVertexIndex) &&
         (textureVertexIndex == other.textureVertexIndex);
}
bool ModelFace::operator==(const ModelFace& other) const {
  for (u32 index = 0; index < faceVertices.size(); ++index) {
    if (faceVertices[index] != other.faceVertices[index]) {
      return false;
    }
  }

  return true;
}

}  // namespace file

namespace std {

size_t hash<file::ModelFaceVertex>::operator()(
    const file::ModelFaceVertex faceVertex) const {
  return std::hash<u32>()(faceVertex.vertexIndex) ^
         std::hash<u32>()(faceVertex.normalVertexIndex) ^
         std::hash<u32>()(faceVertex.textureVertexIndex);
}

size_t hash<file::ModelFace>::operator()(const file::ModelFace& face) const {
  size_t hash = 0;

  for (u32 index = 0; index < face.faceVertices.size(); ++index) {
    hash ^= std::hash<file::ModelFaceVertex>()(face.faceVertices[index]);
  }

  return hash;
}

}  // namespace std
