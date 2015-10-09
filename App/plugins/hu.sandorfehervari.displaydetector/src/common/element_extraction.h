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

Point calculateRectangleCenter(const Rect&);
Rect extractIndicator(Mat& hsvInput);
Rect extractNumberPlate(cv::Mat& hsvInputImage, cv::Mat& dst);
void transformCoordinatesWithBaseline(const cv::Rect baseline, vector<cv::Rect>& boundingBoxes);
void extractNumberFields(cv::Mat& grayInputImage, cv::Rect& numberPlatePlacement, vector<cv::Rect>& boundingBoxes);
void parseHoughCircles(Mat& input, Mat& output);
#endif /* defined(__Csucsaramlasmero__element_extraction__) */

#ifdef TEST
double calculatePercentage(double value, double baseline);
void mergeOverlappingBoxes(const std::vector<cv::Rect> &inputBoxes, cv::Mat &image, std::vector<cv::Rect> &outputBoxes);
#endif