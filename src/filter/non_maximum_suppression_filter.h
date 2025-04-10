/*
 * GPUPixel
 *
 * Created by PixPark on 2021/6/24.
 * Copyright © 2021 PixPark. All rights reserved.
 */

#pragma once

#include "core/gpupixel_define.h"
#include "filter/nearby_sampling3x3_filter.h"

namespace gpupixel {
class GPUPIXEL_API NonMaximumSuppressionFilter
    : public NearbySampling3x3Filter {
 public:
  static std::shared_ptr<NonMaximumSuppressionFilter> Create();
  bool Init();

 protected:
  NonMaximumSuppressionFilter() {};
};

}  // namespace gpupixel
