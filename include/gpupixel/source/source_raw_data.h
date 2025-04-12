/*
 * GPUPixel
 *
 * Created by PixPark on 2021/6/24.
 * Copyright © 2021 PixPark. All rights reserved.
 */

#pragma once

#include <functional>
#include "gpupixel/source/source.h"
#include "gpupixel/filter/filter.h"
namespace gpupixel {
class GPUPIXEL_API SourceRawData : public Filter {
 public:
  virtual ~SourceRawData() = default;
  
  static std::shared_ptr<SourceRawData> Create();

  virtual void ProcessData(const uint8_t* data,
                   int width,
                   int height,
                   int stride,
                   GPUPIXEL_FRAME_TYPE type) = 0;

  virtual void SetRotation(RotationMode rotation) = 0;
};

}  // namespace gpupixel
