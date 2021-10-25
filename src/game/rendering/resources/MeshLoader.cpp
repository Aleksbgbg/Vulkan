#include "MeshLoader.h"

#include <unordered_map>
#include <unordered_set>

#include "TexturedVertex.h"
#include "general/files/images/png.h"
#include "general/files/obj.h"
#include "vulkan/structures/default.h"

MeshLoader::MeshLoader(const VirtualDevice& virtualDevice,
                       DeviceMemoryAllocator& deviceAllocator,
                       const CommandBuffer& commandBuffer, const Fence& fence)
    : virtualDevice(virtualDevice),
      deviceAllocator(deviceAllocator),
      commandBuffer(commandBuffer),
      fence(fence) {}

Mesh MeshLoader::LoadMesh(const MeshLoadParams& params) const {
  std::unordered_set<file::ModelFace> uniqueFaces;
  std::unordered_map<TexturedVertex, u16> uniqueVertices;
  std::vector<TexturedVertex> vertices;
  std::vector<u16> indices;
  std::vector<MeshFrame> meshFrames(params.frames.size());

  glm::vec3 corner1 = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 corner2 = glm::vec3(0.0f, 0.0f, 0.0f);

  for (u32 frameIndex = 0; frameIndex < params.frames.size(); ++frameIndex) {
    const auto& frame = params.frames[frameIndex];
    const file::Model frameModel = file::ModelFromObjFile(frame.model);

    for (const auto& face : frameModel.faces) {
      if (uniqueFaces.contains(face)) {
        continue;
      }

      uniqueFaces.emplace(face);

      for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex) {
        file::ModelFaceVertex modelFaceVertex = face.faceVertices[vertexIndex];

        TexturedVertex vertex;
        vertex.position.x = frameModel.vertices[modelFaceVertex.vertexIndex].x;
        vertex.position.y = frameModel.vertices[modelFaceVertex.vertexIndex].y;
        vertex.position.z = frameModel.vertices[modelFaceVertex.vertexIndex].z;
        vertex.normal.x =
            frameModel.normalVertices[modelFaceVertex.normalVertexIndex].x;
        vertex.normal.y =
            frameModel.normalVertices[modelFaceVertex.normalVertexIndex].y;
        vertex.normal.z =
            frameModel.normalVertices[modelFaceVertex.normalVertexIndex].z;
        vertex.textureCoordinate.x =
            frameModel.textureVertices[modelFaceVertex.textureVertexIndex].u;
        vertex.textureCoordinate.y =
            frameModel.textureVertices[modelFaceVertex.textureVertexIndex].v;

        if (uniqueVertices.contains(vertex)) {
          indices.push_back(uniqueVertices[vertex]);
        } else {
          const u16 index = static_cast<u16>(uniqueVertices.size());

          uniqueVertices.emplace(vertex, index);

          vertices.push_back(vertex);
          indices.push_back(index);

          if (corner1.x < vertex.position.x) {
            corner1.x = vertex.position.x;
          }
          if (corner1.y < vertex.position.y) {
            corner1.y = vertex.position.y;
          }
          if (corner1.z < vertex.position.z) {
            corner1.z = vertex.position.z;
          }
          if (corner2.x > vertex.position.x) {
            corner2.x = vertex.position.x;
          }
          if (corner2.y > vertex.position.y) {
            corner2.y = vertex.position.y;
          }
          if (corner2.z > vertex.position.z) {
            corner2.z = vertex.position.z;
          }
        }
      }
    }

    meshFrames[frameIndex] =
        MeshFrame{.indexCount = static_cast<u32>(indices.size())};
  }

  const glm::vec3 modelCenter = (corner1 + corner2) / 2.0f;
  const glm::vec3 modelSize = corner1 - corner2;

  return Mesh(
      AllocateDeviceBuffer(vertices.data(),
                           sizeof(vertices[0]) * vertices.size(),
                           VK_BUFFER_USAGE_VERTEX_BUFFER_BIT),
      AllocateDeviceBuffer(indices.data(), sizeof(indices[0]) * indices.size(),
                           VK_BUFFER_USAGE_INDEX_BUFFER_BIT),
      LoadTexture(params.texture), LoadTexture(params.emissive),
      std::move(meshFrames), modelCenter, modelSize);
}

Texture MeshLoader::LoadTexture(const std::string_view filename) const {
  const file::Image bitmap = file::ReadPng(filename);

  Buffer stagingBuffer = virtualDevice.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
          .SetSize(bitmap.size)
          .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT));
  DeviceMemorySubAllocation stagingBufferMemory = deviceAllocator.BindMemory(
      stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  stagingBufferMemory.MapCopy(bitmap.data.data(), stagingBuffer.Size());

  Image textureImage =
      virtualDevice.CreateImage(ImageCreateInfoBuilder(IMAGE_2D)
                                    .SetFormat(VK_FORMAT_R8G8B8A8_SRGB)
                                    .SetExtent(Extent3DBuilder()
                                                   .SetWidth(bitmap.width)
                                                   .SetHeight(bitmap.height)
                                                   .SetDepth(1))
                                    .SetUsage(VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                                              VK_IMAGE_USAGE_SAMPLED_BIT));
  DeviceMemorySubAllocation textureImageMemory = deviceAllocator.BindMemory(
      textureImage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  commandBuffer.BeginOneTimeSubmit();
  commandBuffer.CmdImageMemoryBarrier(
      textureImage, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
      VK_PIPELINE_STAGE_TRANSFER_BIT,
      ImageMemoryBarrierBuilder(IMAGE_MEMORY_BARRIER_NO_OWNERSHIP_TRANSFER)
          .SetSrcAccessMask(0)
          .SetDstAccessMask(VK_ACCESS_TRANSFER_WRITE_BIT)
          .SetOldLayout(VK_IMAGE_LAYOUT_UNDEFINED)
          .SetNewLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
          .SetSubresourceRange(SUBRESOURCE_RANGE_COLOR_SINGLE_LAYER));
  commandBuffer.CmdCopyBufferToImage(
      stagingBuffer, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      BufferImageCopyBuilder()
          .SetImageSubresource(SUBRESOURCE_LAYERS_COLOR_SINGLE_LAYER)
          .SetImageExtent(Extent3DBuilder(EXTENT3D_SINGLE_DEPTH)
                              .SetWidth(bitmap.width)
                              .SetHeight(bitmap.height)));
  commandBuffer.CmdImageMemoryBarrier(
      textureImage, VK_PIPELINE_STAGE_TRANSFER_BIT,
      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      ImageMemoryBarrierBuilder(IMAGE_MEMORY_BARRIER_NO_OWNERSHIP_TRANSFER)
          .SetSrcAccessMask(VK_ACCESS_TRANSFER_WRITE_BIT)
          .SetDstAccessMask(VK_ACCESS_SHADER_READ_BIT)
          .SetOldLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
          .SetNewLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
          .SetSubresourceRange(SUBRESOURCE_RANGE_COLOR_SINGLE_LAYER));
  commandBuffer.End().Submit(fence).Wait().Reset();

  ImageView textureView = textureImage.CreateView(
      ImageViewCreateInfoBuilder()
          .SetViewType(VK_IMAGE_VIEW_TYPE_2D)
          .SetFormat(VK_FORMAT_R8G8B8A8_SRGB)
          .SetSubresourceRange(SUBRESOURCE_RANGE_COLOR_SINGLE_LAYER));

  return Texture{.image = {.image = std::move(textureImage),
                           .memory = std::move(textureImageMemory)},
                 .view = std::move(textureView)};
}

BufferWithMemory MeshLoader::AllocateDeviceBuffer(
    const void* const data, const std::size_t size,
    const VkBufferUsageFlags usage) const {
  Buffer stagingBuffer = virtualDevice.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
          .SetSize(size)
          .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT));
  DeviceMemorySubAllocation stagingBufferMemory = deviceAllocator.BindMemory(
      stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  stagingBufferMemory.MapCopy(data, stagingBuffer.Size());

  Buffer finalBuffer = virtualDevice.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
          .SetSize(size)
          .SetUsage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage));
  DeviceMemorySubAllocation finalBufferMemory = deviceAllocator.BindMemory(
      finalBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  commandBuffer.BeginOneTimeSubmit();
  commandBuffer.CmdCopyBufferFull(stagingBuffer, finalBuffer);
  commandBuffer.End().Submit(fence).Wait().Reset();

  return {.buffer = std::move(finalBuffer),
          .memory = std::move(finalBufferMemory)};
}
