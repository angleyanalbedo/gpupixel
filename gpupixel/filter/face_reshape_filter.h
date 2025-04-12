/*
 * GPUPixel
 *
 * Created by PixPark on 2021/6/24.
 * Copyright © 2021 PixPark. All rights reserved.
 */

#pragma once

#include "gpupixel/filter/filter.h"

namespace gpupixel {
class GPUPIXEL_API FaceReshapeFilter : public Filter {
 public:
  static std::shared_ptr<FaceReshapeFilter> Create();
  virtual ~FaceReshapeFilter() = default;
  virtual bool Init() = 0;
  virtual bool DoRender(bool updateSinks = true) = 0;

  virtual void SetFaceSlimLevel(float level) = 0;
  virtual void SetEyeZoomLevel(float level) = 0;
  virtual void SetFaceLandmarks(std::vector<float> landmarks) = 0;
};

}  // namespace gpupixel
