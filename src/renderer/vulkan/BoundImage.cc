#include "BoundImage.h"

BoundImage::BoundImage(Image image, BoundDeviceMemory memory)
    : image_(std::move(image)), memory_(std::move(memory)) {}

ImageView BoundImage::CreateView(
    const ImageViewCreateInfoBuilder& infoBuilder) const {
  return image_.CreateView(infoBuilder);
}
