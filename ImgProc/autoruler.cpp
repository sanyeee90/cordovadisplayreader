//
//  autoruler.cpp
//  rulerDetect
//
//  Created by Sándor Fehérvári on 2015.03.02..
//  Copyright (c) 2015 sandorfehervari. All rights reserved.
//

#include "autoruler.h"
#include <opencv2/highgui/highgui.hpp>
using namespace std;

AutoRuler::AutoRuler(Mat& input) {
    this->inputImage = &input;
}

void AutoRuler::generateImage() {
    generateBlocksFromImage();
}

void AutoRuler::generateBlocksFromImage() {
    
    this->samplesPerWidth = this->inputImage->cols / BLOCK_SIZE;
    this->samplesPerHeight = this->inputImage->rows / BLOCK_SIZE;
    this->imageBlocks = new cv::Mat*[samplesPerWidth];
    for (int i = 0; i< samplesPerWidth;i++){
        this->imageBlocks[i] = new cv::Mat[samplesPerHeight];
    }
    
    
    for (int i = 0;i < samplesPerWidth; i++) {
        for (int j = 0; j<samplesPerHeight; j++) {
            Mat roi2(*this->inputImage,Rect(i*BLOCK_SIZE, j*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE));
            roi2.copyTo(this->imageBlocks[i][j]);
        }
    }
}