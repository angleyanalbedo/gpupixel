/*
 * GPUPixel
 *
 * Created by PixPark on 2021/6/24.
 * Copyright © 2021 PixPark. All rights reserved.
 */

#pragma once

#include <string>

#include "gpupixel/source/source.h"

namespace gpupixel {
class GPUPIXEL_API SourceImage : public Source {
 public:
  virtual ~SourceImage() = default;

  static std::shared_ptr<SourceImage> Create(const std::string name);

  static std::shared_ptr<SourceImage> CreateFromBuffer(
      int width,
      int height,
      int channel_count,
      const unsigned char* pixels);

  virtual const unsigned char* GetRgbaImageBuffer() const = 0;
  virtual int GetWidth() const = 0;
  virtual int GetHeight() const = 0;

  virtual void Render() = 0;
};

}  // namespace gpupixel
