#include "ImageView.h"

#include "error.h"

ImageView::ImageView(VkDevice device, ImageViewCreateInfoBuilder& infoBuilder)
    : device(device) {
  PROCEED_ON_VALID_RESULT(
      vkCreateImageView(device, infoBuilder.Build(), nullptr, &imageView));
}

ImageView::~ImageView() {
  if (imageView != nullptr) {
    vkDestroyImageView(device, imageView, nullptr);
  }
}
