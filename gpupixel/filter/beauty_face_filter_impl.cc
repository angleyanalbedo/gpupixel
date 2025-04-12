/*
 * GPUPixel
 *
 * Created by PixPark on 2021/6/24.
 * Copyright © 2021 PixPark. All rights reserved.
 */

#include "gpupixel/filter/beauty_face_filter_impl.h"
#include "gpupixel/core/gpupixel_context.h"
namespace gpupixel {

BeautyFaceFilterImpl::BeautyFaceFilterImpl() {}

BeautyFaceFilterImpl::~BeautyFaceFilterImpl() {}

std::shared_ptr<BeautyFaceFilter> BeautyFaceFilter::Create() {
  auto ret = std::shared_ptr<BeautyFaceFilterImpl>(new BeautyFaceFilterImpl());
  gpupixel::GPUPixelContext::GetInstance()->SyncRunWithContext([&] {
    if (ret && !ret->Init()) {
      ret.reset();
    }
  });
  return ret;
}

bool BeautyFaceFilterImpl::Init() {
  if (!FilterGroup::Init()) {
    return false;
  }

  box_blur_filter_ = BoxBlurFilter::Create();
  AddFilter(box_blur_filter_);

  box_high_pass_filter_ = BoxHighPassFilter::Create();
  AddFilter(box_high_pass_filter_);

  beauty_face_filter_ = BeautyFaceUnitFilter::Create();
  AddFilter(beauty_face_filter_);

  box_blur_filter_->AddSink(beauty_face_filter_, 1);
  box_high_pass_filter_->AddSink(beauty_face_filter_, 2);

  SetTerminalFilter(beauty_face_filter_);

  box_blur_filter_->SetTexelSpacingMultiplier(4);
  SetRadius(4);

  RegisterProperty("whiteness", 0,
                   "The whiteness of filter with range between -1 and 1.",
                   [this](float& val) { SetWhite(val); });

  RegisterProperty("skin_smoothing", 0,
                   "The smoothing of filter with range between -1 and 1.",
                   [this](float& val) { SetBlurAlpha(val); });
  return true;
}

void BeautyFaceFilterImpl::SetInputFramebuffer(
    std::shared_ptr<GPUPixelFramebuffer> framebuffer,
    RotationMode rotation_mode /* = NoRotation*/,
    int texIdx /* = 0*/) {
  for (auto& filter : filters_) {
    filter->SetInputFramebuffer(framebuffer, rotation_mode, texIdx);
  }
}

void BeautyFaceFilterImpl::SetHighPassDelta(float highPassDelta) {
  box_high_pass_filter_->SetDelta(highPassDelta);
}

void BeautyFaceFilterImpl::SetSharpen(float sharpen) {
  beauty_face_filter_->SetSharpen(sharpen);
}

void BeautyFaceFilterImpl::SetBlurAlpha(float blurAlpha) {
  beauty_face_filter_->SetBlurAlpha(blurAlpha);
}

void BeautyFaceFilterImpl::SetWhite(float white) {
  beauty_face_filter_->SetWhite(white);
}

void BeautyFaceFilterImpl::SetRadius(float radius) {
  box_blur_filter_->SetRadius(radius);
  box_high_pass_filter_->SetRadius(radius);
}
}  // namespace gpupixel 