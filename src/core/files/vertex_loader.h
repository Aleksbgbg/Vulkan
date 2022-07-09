#ifndef SRC_CORE_FILES_VERTEX_LOADER_H_
#define SRC_CORE_FILES_VERTEX_LOADER_H_

#include <unordered_map>

#include "DrawVertices.h"
#include "asset.h"
#include "core/files/obj.h"

template <typename TVertex, typename TVertexProducer>
DrawVertices<TVertex> LoadDrawVertices(const asset::Model modelAsset,
                                       TVertexProducer vertexProducer) {
  std::unordered_map<TVertex, u16> uniqueVertices;
  DrawVertices<TVertex> drawVertices;

  const file::Model model = file::ModelFromAsset(modelAsset);

  for (const auto& face : model.faces) {
    for (u32 vertexIndex = 0; vertexIndex < 3; ++vertexIndex) {
      const file::ModelFaceVertex& modelFaceVertex =
          face.faceVertices[vertexIndex];

      const TVertex vertex = vertexProducer(
          model.vertices[modelFaceVertex.vertexIndex],
          model.normalVertices.empty()
              ? file::ModelNormalVertex()
              : model.normalVertices[modelFaceVertex.normalVertexIndex],
          model.textureVertices.empty()
              ? file::ModelTextureVertex()
              : model.textureVertices[modelFaceVertex.textureVertexIndex]);

      if (uniqueVertices.contains(vertex)) {
        drawVertices.indices.push_back(uniqueVertices[vertex]);
      } else {
        const u16 index = static_cast<u16>(uniqueVertices.size());

        uniqueVertices.insert({vertex, index});

        drawVertices.vertices.push_back(vertex);
        drawVertices.indices.push_back(index);
      }
    }
  }

  return drawVertices;
}

#endif  // SRC_CORE_FILES_VERTEX_LOADER_H_
