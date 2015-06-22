//
//  constants.h
//  Csucsaramlasmero
//
//  Created by Fehervari Sandor on 2015. 03. 28..
//  Copyright (c) 2015. sandorfehervari. All rights reserved.
//

#ifndef Csucsaramlasmero_constants_h
#define Csucsaramlasmero_constants_h

#include "opencv2/core/core.hpp"

using namespace cv;

static const int COMMON_SUCCESS = 0;
static const int INDICATOR_DETECTION_FAILED = 1;
static const int BLOB_DETECTION_FAILED = 2;

static const Scalar RED_RANGE_START(0,90,130);
static const Scalar RED_RANGE_END(12,255,241);

static const Scalar YELLOW_RANGE_START(18,100,120);
static const Scalar YELLOW_RANGE_END(30,255,230);

static const Matx<uint, 3, 3> INDICATOR_ERODE_KERNEL(0,1,0,1,1,1,0,1,0);

#endif
