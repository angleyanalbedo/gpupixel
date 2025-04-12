/*
 * GPUPixel
 *
 * Created by PixPark on 2021/6/24.
 * Copyright © 2021 PixPark. All rights reserved.
 */

#include "gpupixel/source/source_image_imp.h"
#include "gpupixel/core/gpupixel_context.h"
#include "gpupixel/utils/util.h"

#if defined(GPUPIXEL_ANDROID)
#include <android/bitmap.h>
#include "gpupixel/android/jni/jni_helpers.h"
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "third_party/stb/stb_image.h"

namespace gpupixel {

std::shared_ptr<SourceImage> SourceImage::CreateFromBuffer(
    int width,
    int height,
    int channel_count,
    const unsigned char* pixels) {
  auto sourceImage = std::shared_ptr<SourceImageImpl>(new SourceImageImpl());
  gpupixel::GPUPixelContext::GetInstance()->SyncRunWithContext(
      [&] { sourceImage->Init(width, height, channel_count, pixels); });
  return sourceImage;
}

std::shared_ptr<SourceImage> SourceImage::Create(const std::string name) {
  int width, height, channel_count;
  unsigned char* data =
      stbi_load(name.c_str(), &width, &height, &channel_count, 4);
  //   todo(logo info)
  if (data == nullptr) {
    Util::Log("SourceImage", "SourceImage: input data in null! file name: %s",
              name.c_str());
    return nullptr;
  }
  auto image =
      SourceImage::CreateFromBuffer(width, height, channel_count, data);
  stbi_image_free(data);
  return image;
}

void SourceImageImpl::Init(int width,
                       int height,
                       int channel_count,
                       const unsigned char* pixels) {
  this->SetFramebuffer(0);
  if (!framebuffer_ || (framebuffer_->GetWidth() != width ||
                        framebuffer_->GetHeight() != height)) {
    framebuffer_ = GPUPixelContext::GetInstance()
                       ->GetFramebufferFactory()
                       ->CreateFramebuffer(width, height, true);
  }
  this->SetFramebuffer(framebuffer_);
  CHECK_GL(glBindTexture(GL_TEXTURE_2D, this->GetFramebuffer()->GetTexture()));

  CHECK_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                        GL_UNSIGNED_BYTE, pixels));
  image_bytes_.assign(pixels, pixels + width * height * 4);

  CHECK_GL(glBindTexture(GL_TEXTURE_2D, 0));
}

void SourceImageImpl::Render() {
  gpupixel::GPUPixelContext::GetInstance()->SyncRunWithContext(
      [&] { Source::DoRender(); });
}

const unsigned char* SourceImageImpl::GetRgbaImageBuffer() const {
  return image_bytes_.data();
}

int SourceImageImpl::GetWidth() const {
  if (framebuffer_) {
    return framebuffer_->GetWidth();
  }
  return 0;
}

int SourceImageImpl::GetHeight() const {
  if (framebuffer_) {
    return framebuffer_->GetHeight();
  }
  return 0;
}

}  // namespace gpupixel 