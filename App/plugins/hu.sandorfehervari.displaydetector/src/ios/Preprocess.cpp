//
//  Preprocess.cpp
//  Analog Display Reader
//
//  Created by Sándor Fehérvári on 2015.02.12..
//
//

#include "Preprocess.h"

void preprocessImage(cv::Mat& inputMat, cv::Mat& processed) {
    cv::cvtColor(inputMat, processed, CV_RGB2GRAY);
}