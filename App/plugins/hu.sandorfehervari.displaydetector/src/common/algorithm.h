//
//  algorithm.h
//  Csucsaramlasmero
//
//  Created by Fehervari Sandor on 2015. 06. 21..
//  Copyright (c) 2015. sandorfehervari. All rights reserved.
//

#ifndef __Csucsaramlasmero__algorithm__
#define __Csucsaramlasmero__algorithm__

#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;

class PFMResultDetector {
private:
    Mat inputImage;
    Mat grayImage;
    Mat hsvImage;
    Mat filtered_gray;
    vector<cv::Rect> boundRect;
    Rect indicatorRect;
    vector<pair<cv::Point, int> > pointsWithNumbers;
    
    const char* tessDataDir;
    
    void detectIndicatorOnOriginalImage();
    void detectNumberFields();
    void readNumbersFromImage();
public:
    PFMResultDetector(Mat& input, const char* tessDataDir = "");
    float runAlgorithm();
};


#endif /* defined(__Csucsaramlasmero__algorithm__) */
