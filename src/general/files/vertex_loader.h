#ifndef VULKAN_SRC_GENERAL_GEOMETRY_VERTEXLOADER_H
#define VULKAN_SRC_GENERAL_GEOMETRY_VERTEXLOADER_H

#include <string_view>
#include <unordered_map>

#include "DrawVertices.h"
#include "general/files/obj.h"

template <typename TVertex, typename TVertexProducer>
DrawVertices<TVertex> LoadUniqueDrawVertices(const std::string_view filename,
                                             TVertexProducer vertexProducer) {
  std::unordered_map<TVertex, u16> uniqueVertices;
  DrawVertices<TVertex> drawVertices;

  const file::Model model = file::ModelFromObjFile(filename);

  for (const auto& face : model.faces) {
    for (u32 vertexIndex = 0; vertexIndex < 3; ++vertexIndex) {
      const file::ModelFaceVertex& modelFaceVertex =
          face.faceVertices[vertexIndex];

      const TVertex vertex = vertexProducer(
          model.vertices[modelFaceVertex.vertexIndex],
          model.normalVertices[modelFaceVertex.normalVertexIndex],
          model.textureVertices[modelFaceVertex.textureVertexIndex]);

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

#endif  // VULKAN_SRC_GENERAL_GEOMETRY_VERTEXLOADER_H
