//
//  source_raw_output.h
//  GPUPixel iOS
//
//  Created by PixPark on 2021/6/24.
//  Copyright © 2021 PixPark. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <functional>
#include <memory>
#include <mutex>
#include <string>

#include "gpupixel/sink/sink.h"

#if defined(GPUPIXEL_IOS) || defined(GPUPIXEL_MAC)
#import <AVFoundation/AVFoundation.h>
#import <Foundation/Foundation.h>
#endif

namespace gpupixel {

class GPUPIXEL_API SinkRawData : public Sink {
 public:
  virtual ~SinkRawData() = default;
  static std::shared_ptr<SinkRawData> Create();
  virtual void Render() override = 0;

  virtual const uint8_t* GetRgbaBuffer() = 0;
  virtual const uint8_t* GetI420Buffer() = 0;
  virtual int GetWidth() const = 0;
  virtual int GetHeight() const = 0;
};

}  // namespace gpupixel
