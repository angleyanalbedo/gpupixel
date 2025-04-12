/*
 * GPUPixel
 *
 * Created by PixPark on 2021/6/24.
 * Copyright © 2021 PixPark. All rights reserved.
 */

#pragma once

#include "gpupixel/filter/filter_group.h"
#include "gpupixel/core/gpupixel_define.h"

namespace gpupixel {

class GPUPIXEL_API BeautyFaceFilter : public FilterGroup {
 public:
  static std::shared_ptr<BeautyFaceFilter> Create();
  virtual ~BeautyFaceFilter() = default;

  virtual void SetHighPassDelta(float highPassDelta) = 0;
  virtual void SetSharpen(float sharpen) = 0;
  virtual void SetBlurAlpha(float blurAlpha) = 0;
  virtual void SetWhite(float white) = 0;
  virtual void SetRadius(float sigma) = 0;

  virtual void SetInputFramebuffer(
      std::shared_ptr<GPUPixelFramebuffer> framebuffer,
      RotationMode rotation_mode = NoRotation,
      int texIdx = 0) = 0;
};

}  // namespace gpupixel
