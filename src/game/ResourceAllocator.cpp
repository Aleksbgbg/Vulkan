#include "ResourceAllocator.h"

#include "general/files/images/png.h"
#include "vulkan/structures/default.h"

ResourceAllocator::ResourceAllocator(const VirtualDevice& virtualDevice,
                                     DeviceMemoryAllocator& deviceAllocator,
                                     const CommandBuffer& commandBuffer,
                                     const Fence& fence)
    : virtualDevice(&virtualDevice),
      deviceAllocator(&deviceAllocator),
      commandBuffer(&commandBuffer),
      fence(&fence) {}

Texture ResourceAllocator::LoadTexture(const std::string_view filename) const {
  const file::Image image = file::ReadPng(filename);

  Buffer stagingBuffer = virtualDevice->CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
          .SetSize(image.size)
          .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT));
  DeviceMemorySubAllocation stagingBufferMemory = deviceAllocator->BindMemory(
      stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  stagingBufferMemory.MapCopy(image.data.data(), stagingBuffer.Size());

  Image textureImage =
      virtualDevice->CreateImage(ImageCreateInfoBuilder(IMAGE_2D)
                                     .SetFormat(VK_FORMAT_R8G8B8A8_SRGB)
                                     .SetExtent(Extent3DBuilder()
                                                    .SetWidth(image.width)
                                                    .SetHeight(image.height)
                                                    .SetDepth(1))
                                     .SetUsage(VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                                               VK_IMAGE_USAGE_SAMPLED_BIT));
  DeviceMemorySubAllocation textureImageMemory = deviceAllocator->BindMemory(
      textureImage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  commandBuffer->BeginOneTimeSubmit();
  commandBuffer->CmdImageMemoryBarrier(
      textureImage, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
      VK_PIPELINE_STAGE_TRANSFER_BIT,
      ImageMemoryBarrierBuilder(IMAGE_MEMORY_BARRIER_NO_OWNERSHIP_TRANSFER)
          .SetSrcAccessMask(0)
          .SetDstAccessMask(VK_ACCESS_TRANSFER_WRITE_BIT)
          .SetOldLayout(VK_IMAGE_LAYOUT_UNDEFINED)
          .SetNewLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
          .SetSubresourceRange(SUBRESOURCE_RANGE_COLOR_SINGLE_LAYER));
  commandBuffer->CmdCopyBufferToImage(
      stagingBuffer, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      BufferImageCopyBuilder()
          .SetImageSubresource(SUBRESOURCE_LAYERS_COLOR_SINGLE_LAYER)
          .SetImageExtent(Extent3DBuilder(EXTENT3D_SINGLE_DEPTH)
                              .SetWidth(image.width)
                              .SetHeight(image.height)));
  commandBuffer->CmdImageMemoryBarrier(
      textureImage, VK_PIPELINE_STAGE_TRANSFER_BIT,
      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      ImageMemoryBarrierBuilder(IMAGE_MEMORY_BARRIER_NO_OWNERSHIP_TRANSFER)
          .SetSrcAccessMask(VK_ACCESS_TRANSFER_WRITE_BIT)
          .SetDstAccessMask(VK_ACCESS_SHADER_READ_BIT)
          .SetOldLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
          .SetNewLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
          .SetSubresourceRange(SUBRESOURCE_RANGE_COLOR_SINGLE_LAYER));
  commandBuffer->End().Submit(*fence).Wait().Reset();

  ImageView textureView = textureImage.CreateView(
      ImageViewCreateInfoBuilder()
          .SetViewType(VK_IMAGE_VIEW_TYPE_2D)
          .SetFormat(VK_FORMAT_R8G8B8A8_SRGB)
          .SetSubresourceRange(SUBRESOURCE_RANGE_COLOR_SINGLE_LAYER));

  return Texture{.image = {.image = std::move(textureImage),
                           .memory = std::move(textureImageMemory)},
                 .view = std::move(textureView)};
}

BufferWithMemory ResourceAllocator::AllocateLocalBuffer(
    const std::size_t size, const VkBufferUsageFlags usage) const {
  BufferWithMemory bufferWithMemory;
  bufferWithMemory.buffer = virtualDevice->CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE).SetSize(size).SetUsage(usage));
  bufferWithMemory.memory = deviceAllocator->BindMemory(
      bufferWithMemory.buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  return bufferWithMemory;
}

BufferWithMemory ResourceAllocator::AllocateDeviceBuffer(
    const void* const data, const std::size_t size,
    const VkBufferUsageFlags usage) const {
  Buffer stagingBuffer = virtualDevice->CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
          .SetSize(size)
          .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT));
  DeviceMemorySubAllocation stagingBufferMemory = deviceAllocator->BindMemory(
      stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  stagingBufferMemory.MapCopy(data, stagingBuffer.Size());

  Buffer finalBuffer = virtualDevice->CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
          .SetSize(size)
          .SetUsage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage));
  DeviceMemorySubAllocation finalBufferMemory = deviceAllocator->BindMemory(
      finalBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  commandBuffer->BeginOneTimeSubmit();
  commandBuffer->CmdCopyBufferFull(stagingBuffer, finalBuffer);
  commandBuffer->End().Submit(*fence).Wait().Reset();

  return {.buffer = std::move(finalBuffer),
          .memory = std::move(finalBufferMemory)};
}
