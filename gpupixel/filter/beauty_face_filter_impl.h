/*
 * GPUPixel
 *
 * Created by PixPark on 2021/6/24.
 * Copyright © 2021 PixPark. All rights reserved.
 */

#pragma once

#include "gpupixel/filter/beauty_face_filter.h"
#include "gpupixel/filter/beauty_face_unit_filter.h"
#include "gpupixel/filter/box_blur_filter.h"
#include "gpupixel/filter/box_high_pass_filter.h"
#include "gpupixel/filter/gaussian_blur_filter.h"

namespace gpupixel {
class GPUPIXEL_API BeautyFaceFilterImpl : public BeautyFaceFilter {
 public:
  BeautyFaceFilterImpl();
  ~BeautyFaceFilterImpl();
  
  bool Init();

  void SetHighPassDelta(float highPassDelta) override;
  void SetSharpen(float sharpen) override;
  void SetBlurAlpha(float blurAlpha) override;
  void SetWhite(float white) override;
  void SetRadius(float sigma) override;

  virtual void SetInputFramebuffer(
      std::shared_ptr<GPUPixelFramebuffer> framebuffer,
      RotationMode rotation_mode = NoRotation,
      int texIdx = 0) override;

 private:
  std::shared_ptr<BoxBlurFilter> box_blur_filter_;
  std::shared_ptr<BoxHighPassFilter> box_high_pass_filter_;
  std::shared_ptr<BeautyFaceUnitFilter> beauty_face_filter_;
};

}  // namespace gpupixel 