//
//  element_extraction.h
//  Csucsaramlasmero
//
//  Created by Fehervari Sandor on 2015. 03. 28..
//  Copyright (c) 2015. sandorfehervari. All rights reserved.
//

#ifndef __Csucsaramlasmero__element_extraction__
#define __Csucsaramlasmero__element_extraction__

#include "opencv2/core/core.hpp"
using namespace cv;
Point extractIndicator(Mat& hsvInput);
Rect extractNumberPlate(cv::Mat& hsvInputImage, cv::Mat& dst);
void extractNumberFields(cv::Mat& grayInputImage, cv::Rect& numberPlatePlacement, cv::Mat& dst, vector<cv::Rect>& boundingBoxes);
void parseHoughCircles(Mat& input, Mat& output);
#endif /* defined(__Csucsaramlasmero__element_extraction__) */
