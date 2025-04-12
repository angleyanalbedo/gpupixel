/*
 * GPUPixel
 *
 * Created by PixPark on 2021/6/24.
 * Copyright © 2021 PixPark. All rights reserved.
 */

#pragma once

#include <stdlib.h>
#include <functional>
#include <vector>
#include "gpupixel/core/gpupixel_define.h"

namespace gpupixel {

class GPUPIXEL_API FaceDetector {
 public:
  static std::shared_ptr<FaceDetector> Create();
  virtual ~FaceDetector() = default;

  virtual std::vector<float> Detect(const uint8_t* data,
                                    int width,
                                    int height,
                                    int stride,
                                    GPUPIXEL_MODE_FMT fmt,
                                    GPUPIXEL_FRAME_TYPE type) = 0;
};
}  // namespace gpupixel
