
/*
 * GPUPixel
 *
 * Created by PixPark on 2021/6/24.
 * Copyright © 2021 PixPark. All rights reserved.
 */

#pragma once


// core
#include "core/gpupixel_define.h"
#include "core/gpupixel_context.h"
#include "core/gpupixel_framebuffer.h"
#include "core/gpupixel_framebuffer_factory.h"
#include "core/gpupixel_program.h"

// utils
#include "utils/math_toolbox.h"
#include "utils/util.h"

// source
#include "source/source.h"
#include "source/source_image.h"
#include "source/source_raw_data.h"

// sink
#include "sink/sink.h"
#include "sink/sink_raw_data.h"
#include "sink/sink_render.h"
#if defined(GPUPIXEL_IOS) || defined(GPUPIXEL_MAC)
#include "sink/objc/gpupixel_sink.h"
#include "sink/objc/gpupixel_view.h"
#include "sink/objc/objc_sink.h"
#endif

// face detect
#include "face_detector/face_detector.h"

// base filters
#include "filter/filter.h"
#include "filter/filter_group.h"

// face filters
#include "filter/beauty_face_filter.h"
#include "filter/blusher_filter.h"
#include "filter/face_makeup_filter.h"
#include "filter/face_reshape_filter.h"
#include "filter/lipstick_filter.h"

// general filters
#include "filter/bilateral_filter.h"
#include "filter/box_blur_filter.h"
#include "filter/box_high_pass_filter.h"
#include "filter/brightness_filter.h"
#include "filter/canny_edge_detection_filter.h"
#include "filter/color_invert_filter.h"
#include "filter/color_matrix_filter.h"
#include "filter/contrast_filter.h"
#include "filter/convolution3x3_filter.h"
#include "filter/crosshatch_filter.h"
#include "filter/directional_non_maximum_suppression_filter.h"
#include "filter/directional_sobel_edge_detection_filter.h"
#include "filter/emboss_filter.h"
#include "filter/exposure_filter.h"
#include "filter/gaussian_blur_filter.h"
#include "filter/gaussian_blur_mono_filter.h"
#include "filter/glass_sphere_filter.h"
#include "filter/grayscale_filter.h"
#include "filter/halftone_filter.h"
#include "filter/hsb_filter.h"
#include "filter/hue_filter.h"
#include "filter/ios_blur_filter.h"
#include "filter/luminance_range_filter.h"
#include "filter/nearby_sampling3x3_filter.h"
#include "filter/non_maximum_suppression_filter.h"
#include "filter/pixellation_filter.h"
#include "filter/posterize_filter.h"
#include "filter/rgb_filter.h"
#include "filter/saturation_filter.h"
#include "filter/single_component_gaussian_blur_filter.h"
#include "filter/single_component_gaussian_blur_mono_filter.h"
#include "filter/sketch_filter.h"
#include "filter/smooth_toon_filter.h"
#include "filter/sobel_edge_detection_filter.h"
#include "filter/sphere_refraction_filter.h"
#include "filter/toon_filter.h"
#include "filter/weak_pixel_inclusion_filter.h"
#include "filter/white_balance_filter.h"
