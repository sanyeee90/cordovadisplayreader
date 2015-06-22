//
//  utils.h
//  Csucsaramlasmero
//
//  Created by Fehervari Sandor on 2015. 03. 28..
//  Copyright (c) 2015. sandorfehervari. All rights reserved.
//

#ifndef __Csucsaramlasmero__utils__
#define __Csucsaramlasmero__utils__

#include <opencv2/core/core.hpp>

bool compareByHeight(std::pair<cv::Point, int> p1, std::pair<cv::Point, int> p2);

cv::Point calculateCenterOfRectangle(cv::Rect rectangle);

int findBiggestBlob(cv::Mat &src, cv::Mat &dst);

#endif /* defined(__Csucsaramlasmero__utils__) */
