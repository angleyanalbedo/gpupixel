/*
 * GPUPixel
 *
 * Created by PixPark on 2021/6/24.
 * Copyright © 2021 PixPark. All rights reserved.
 */

#pragma once

#include <functional>
#include "gpupixel/source/source_raw_data.h"
#include "gpupixel/core/gpupixel_program.h"
#include "gpupixel/filter/filter.h"

namespace gpupixel {
class GPUPIXEL_API SourceRawDataImpl : public SourceRawData, public Filter {
 public:
  SourceRawDataImpl();
  ~SourceRawDataImpl() override;
  
  void ProcessData(const uint8_t* data,
                   int width,
                   int height,
                   int stride,
                   GPUPIXEL_FRAME_TYPE type) override;

  void SetRotation(RotationMode rotation) override;
  
  bool Init();

 private:
  int GenerateTextureWithI420(int width,
                              int height,
                              const uint8_t* dataY,
                              int strideY,
                              const uint8_t* dataU,
                              int strideU,
                              const uint8_t* dataV,
                              int strideV);

  int GenerateTextureWithPixels(const uint8_t* pixels,
                                int width,
                                int height,
                                int stride,
                                GPUPIXEL_FRAME_TYPE type);

 private:
  GPUPixelGLProgram* filter_program_;
  GLuint filter_position_attribute_;
  GLuint filter_tex_coord_attribute_;

  GLuint textures_[4] = {0};
  RotationMode rotation_ = NoRotation;
  std::shared_ptr<GPUPixelFramebuffer> framebuffer_;
};

}  // namespace gpupixel 