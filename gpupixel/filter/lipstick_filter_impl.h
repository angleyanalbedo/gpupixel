/*
 * GPUPixel
 *
 * Created by PixPark on 2021/6/24.
 * Copyright © 2021 PixPark. All rights reserved.
 */

#pragma once

#include "gpupixel/filter/lipstick_filter.h"

namespace gpupixel {
class GPUPIXEL_API LipstickFilterImpl : public LipstickFilter {
 public:
  LipstickFilterImpl();
  
  bool Init() override;
};

}  // namespace gpupixel 