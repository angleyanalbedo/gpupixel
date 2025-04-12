/*
 * GPUPixel
 *
 * Created by PixPark on 2021/6/24.
 * Copyright © 2021 PixPark. All rights reserved.
 */

#pragma once

#include "gpupixel/sink/sink.h"

namespace gpupixel {
class GPUPIXEL_API SinkRender : public Sink {
 public:
  enum FillMode {
    Stretch = 0,  // Stretch to fill the view, and may distort the image
    PreserveAspectRatio = 1,  // preserve the aspect ratio of the image
    PreserveAspectRatioAndFill =
        2  // preserve the aspect ratio, and zoom in to fill the view
  };

 public:
  static std::shared_ptr<SinkRender> Create();
  virtual ~SinkRender() = default;

  virtual void Init() = 0;
  virtual void SetInputFramebuffer(
      std::shared_ptr<GPUPixelFramebuffer> framebuffer,
      RotationMode rotation_mode = NoRotation,
      int tex_idx = 0) override = 0;
  virtual void SetFillMode(FillMode fill_mode) = 0;
  virtual void SetMirror(bool mirror) = 0;
  virtual void SetRenderSize(int width, int height) = 0;
  virtual void Render() override = 0;
};

}  // namespace gpupixel
