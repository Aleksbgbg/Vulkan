#include "Image.h"

#include "DeviceMemory.h"
#include "error.h"

Image::Image() : image(nullptr) {}

Image::Image(VkDevice device, const ImageCreateInfoBuilder& infoBuilder)
    : device(device), createInfo(infoBuilder) {
  PROCEED_ON_VALID_RESULT(vkCreateImage(device, &createInfo, nullptr, &image));
}

Image::Image(Image&& other) noexcept
    : device(other.device), image(other.image), createInfo(other.createInfo) {
  other.image = nullptr;
}

Image::~Image() {
  if (image != nullptr) {
    vkDestroyImage(device, image, nullptr);
  }
}

Image& Image::operator=(Image&& other) noexcept {
  std::swap(device, other.device);
  std::swap(image, other.image);
  createInfo = other.createInfo;
  return *this;
}

VkMemoryRequirements Image::GetMemoryRequirements() const {
  VkMemoryRequirements memoryRequirements;
  vkGetImageMemoryRequirements(device, image, &memoryRequirements);
  return memoryRequirements;
}

ImageView Image::CreateView(ImageViewCreateInfoBuilder infoBuilder) const {
  return ImageView(device,
                   infoBuilder.SetImage(image).SetFormat(createInfo.format));
}
