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

static const Scalar RED_RANGE_START(0,90,130);
static const Scalar RED_RANGE_END(12,255,241);

static const Scalar YELLOW_RANGE_START(23,100,80);
static const Scalar YELLOW_RANGE_END(36,255,255);

static const float NUMBER_FIELD_EXTRA_PERCENT = 0.05f;
static const int OPTIMAL_IMAGE_SIZE = 1280;
static const int AREA_MULTIPLER = 2;

static const int ERROR_INVALID_RESULT = -1;
static const int ERROR_INVALID_INPUT_IMAGE = -2;
static const int ERROR_EMPTY_IMAGE = -100;
static const int ERROR_FAILED_TO_DETECT_NUMNER_PLATE = -200;
static const int ERROR_FAILED_TO_DETECT_INDICATOR = -300;
static const int ERROR_FAILED_TO_DETECT_NUMBER_FIELDS = -400;
static const int ERROR_FAILED_TO_DETECT_NUMBER_FIELDS_DIGIT = -500;






//static const Matx<uint, 3, 3> INDICATOR_ERODE_KERNEL(0,1,0,1,1,1,0,1,0);

#endif
