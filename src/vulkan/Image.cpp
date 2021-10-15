#include "Image.h"

#include <stdexcept>

#include "DeviceMemory.h"
#include "error.h"

Image::Image(VkDevice device,
             VkPhysicalDeviceMemoryProperties* memoryProperties,
             ImageCreateInfoBuilder& infoBuilder)
    : device(device),
      memoryProperties(memoryProperties),
      createInfo(infoBuilder) {
  PROCEED_ON_VALID_RESULT(vkCreateImage(device, &createInfo, nullptr, &image));
}

Image::~Image() {
  if (image != nullptr) {
    vkDestroyImage(device, image, nullptr);
  }
}

VkMemoryRequirements Image::GetMemoryRequirements() const {
  VkMemoryRequirements memoryRequirements;
  vkGetImageMemoryRequirements(device, image, &memoryRequirements);
  return memoryRequirements;
}

ImageView Image::CreateView(ImageViewCreateInfoBuilder& infoBuilder) {
  return ImageView(device,
                   infoBuilder.SetImage(image).SetFormat(createInfo.format));
}
