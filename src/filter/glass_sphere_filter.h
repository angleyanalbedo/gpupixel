/*
 * GPUPixel
 *
 * Created by PixPark on 2021/6/24.
 * Copyright © 2021 PixPark. All rights reserved.
 */

#pragma once

#include "core/gpupixel_define.h"
#include "filter/sphere_refraction_filter.h"

namespace gpupixel {
class GPUPIXEL_API GlassSphereFilter : public SphereRefractionFilter {
 public:
  static std::shared_ptr<GlassSphereFilter> Create();
  bool Init();

 protected:
  GlassSphereFilter() {};
};

}  // namespace gpupixel
