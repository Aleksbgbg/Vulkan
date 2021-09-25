#include <stdexcept>
#include "Image.h"
#include "error.h"
#include "ImageView.h"

Image::Image(VkDevice device, VkPhysicalDeviceMemoryProperties* memoryProperties, ImageCreateInfoBuilder& infoBuilder)
  : device(device), memoryProperties(memoryProperties), createInfo(infoBuilder.BuildObject()) {
  PROCEED_ON_VALID_RESULT(vkCreateImage(device, &createInfo, nullptr, &image))
}

Image::~Image() {
  if (image != nullptr) {
    vkDestroyImage(device, image, nullptr);
  }
}

DeviceMemory Image::AllocateAndBindMemory(const VkMemoryPropertyFlags requiredProperties) const {
  VkMemoryRequirements memoryRequirements;
  vkGetImageMemoryRequirements(device, image, &memoryRequirements);

  std::optional<u32> memoryTypeIndex =
      DeviceMemory::FindSuitableMemoryTypeIndex(
          *memoryProperties, requiredProperties, memoryRequirements.memoryTypeBits);

  if (!memoryTypeIndex.has_value()) {
    throw std::runtime_error("Could not find suitable GPU memory to allocate.");
  }

  return
      std::move(
          DeviceMemory(
              device,
              MemoryAllocateInfoBuilder()
                  .SetAllocationSize(memoryRequirements.size)
                  .SetMemoryTypeIndex(memoryTypeIndex.value()))
              .BindAll(image));
}

ImageView Image::CreateView(ImageViewCreateInfoBuilder& infoBuilder) {
  return ImageView(device, infoBuilder.SetImage(image).SetFormat(createInfo.format));
}

