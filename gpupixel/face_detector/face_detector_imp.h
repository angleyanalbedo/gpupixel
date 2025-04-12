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
#include "gpupixel/face_detector/face_detector.h"

namespace mars_face_kit {
class MarsFaceDetector;
}

namespace gpupixel {

class GPUPIXEL_API FaceDetectorImpl : public FaceDetector {
 public:
  FaceDetectorImpl();
  ~FaceDetectorImpl() override;

  std::vector<float> Detect(const uint8_t* data,
                            int width,
                            int height,
                            int stride,
                            GPUPIXEL_MODE_FMT fmt,
                            GPUPIXEL_FRAME_TYPE type) override;

 private:
  std::shared_ptr<mars_face_kit::MarsFaceDetector> mars_face_detector_;
};
}  // namespace gpupixel
