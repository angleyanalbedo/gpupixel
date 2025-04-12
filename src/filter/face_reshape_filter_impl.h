/*
 * GPUPixel
 *
 * Created by PixPark on 2021/6/24.
 * Copyright © 2021 PixPark. All rights reserved.
 */

#pragma once

#include "gpupixel/filter/face_reshape_filter.h"

namespace gpupixel {
class GPUPIXEL_API FaceReshapeFilterImpl : public FaceReshapeFilter {
 public:
  FaceReshapeFilterImpl();
  ~FaceReshapeFilterImpl();
  
  bool Init() override;
  bool DoRender(bool updateSinks = true) override;

  void SetFaceSlimLevel(float level) override;
  void SetEyeZoomLevel(float level) override;
  void SetFaceLandmarks(std::vector<float> landmarks) override;

 private:
  float thin_face_delta_ = 0.0;
  float big_eye_delta_ = 0.0;

  std::vector<float> face_landmarks_;
  int has_face_ = 0;
};

}  // namespace gpupixel 