#include "IndexedVertexBuffer.h"

IndexedVertexBuffer::IndexedVertexBuffer(BoundBuffer vertexIndexBuffer,
                                         const VkDeviceSize indexDataOffset,
                                         const u32 indexCount)
    : vertexIndexBuffer_(std::move(vertexIndexBuffer)),
      indexDataOffset_(indexDataOffset),
      indexCount_(indexCount) {}

void IndexedVertexBuffer::Draw(const vk::CommandBuffer& commandBuffer,
                               u32 instances) const {
  commandBuffer.CmdBindVertexBuffers(vertexIndexBuffer_.RawBuffer());
  commandBuffer.CmdBindIndexBuffer(vertexIndexBuffer_.RawBuffer(),
                                   indexDataOffset_, VK_INDEX_TYPE_UINT16);
  commandBuffer.CmdDrawIndexedInstanced(indexCount_, instances);
}
