/*
 * GPUPixel
 *
 * Created by PixPark on 2021/6/24.
 * Copyright © 2021 PixPark. All rights reserved.
 */

#pragma once

#include <string>
#include <vector>

#include "gpupixel/source/source_image.h"

namespace gpupixel {
class GPUPIXEL_API SourceImageImpl : public SourceImage {
 public:
  SourceImageImpl() {}
  ~SourceImageImpl() override {};

  const unsigned char* GetRgbaImageBuffer() const override;
  int GetWidth() const override;
  int GetHeight() const override;

  void Render() override;

  void Init(int width,
            int height,
            int channel_count,
            const unsigned char* pixels);

#if defined(GPUPIXEL_ANDROID)
  static std::shared_ptr<SourceImageImpl> CreateImageForAndroid(std::string name);
#endif
  std::vector<unsigned char> image_bytes_;
};

}  // namespace gpupixel 