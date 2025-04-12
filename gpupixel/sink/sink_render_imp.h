/*
 * GPUPixel
 *
 * Created by PixPark on 2021/6/24.
 * Copyright © 2021 PixPark. All rights reserved.
 */

#pragma once

#include "gpupixel/core/gpupixel_program.h"
#include "gpupixel/sink/sink_render.h"

namespace gpupixel {
class GPUPIXEL_API SinkRenderImpl : public SinkRender {
 public:
  SinkRenderImpl();
  ~SinkRenderImpl() override;

  void Init() override;
  void SetInputFramebuffer(
      std::shared_ptr<GPUPixelFramebuffer> framebuffer,
      RotationMode rotation_mode = NoRotation,
      int tex_idx = 0) override;
  void SetFillMode(FillMode fill_mode) override;
  void SetMirror(bool mirror) override;
  void SetRenderSize(int width, int height) override;
  void Render() override;

 private:
  int view_width_;
  int view_height_;
  FillMode fill_mode_;
  bool mirror_ = false;
  GPUPixelGLProgram* display_program_;
  uint32_t position_attribute_location_;
  uint32_t tex_coord_attribute_location_;
  uint32_t color_map_uniform_location_;
  struct {
    float r;
    float g;
    float b;
    float a;
  } background_color_;

  float display_vertices_[8];

  void UpdateDisplayVertices();
  const float* GetTextureCoordinate(RotationMode rotation_mode);
};

}  // namespace gpupixel 