#include "ImageView.h"

#include <utility>

#include "error.h"

namespace vk {

ImageView::ImageView() : imageView(nullptr) {}

ImageView::ImageView(VkDevice device,
                     const ImageViewCreateInfoBuilder& infoBuilder)
    : device(device) {
  PROCEED_ON_VALID_RESULT(
      vkCreateImageView(device, infoBuilder.Build(), nullptr, &imageView));
}

ImageView::ImageView(ImageView&& other) noexcept
    : device(other.device), imageView(other.imageView) {
  other.imageView = nullptr;
}

ImageView::~ImageView() {
  if (imageView != nullptr) {
    vkDestroyImageView(device, imageView, nullptr);
  }
}

ImageView& ImageView::operator=(ImageView&& other) noexcept {
  std::swap(device, other.device);
  std::swap(imageView, other.imageView);
  return *this;
}

}  // namespace vk
