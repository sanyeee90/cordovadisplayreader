
//
//  preprocess_image.cpp
//  Analog Display Reader
//
//  Created by Fehervari Sandor on 2015. 03. 28..
//
//

#include "preprocess_image.h"
#include <opencv2/opencv.hpp>


using namespace cv;

void sharpenImage(Mat& inputImage) {
    Mat blurred;
    GaussianBlur(inputImage, blurred, Size(0,0), 3);
    addWeighted(inputImage, 1.5, blurred, -0.5, 0, inputImage);
}