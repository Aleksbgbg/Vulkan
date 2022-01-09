#include "BoundImage.h"

BoundImage::BoundImage(vk::Image image, vk::BoundDeviceMemory memory)
    : image_(std::move(image)), memory_(std::move(memory)) {}

vk::ImageView BoundImage::CreateView(
    const ImageViewCreateInfoBuilder& infoBuilder) const {
  return image_.CreateView(infoBuilder);
}
