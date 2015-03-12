//
//  autoruler.h
//  rulerDetect
//
//  Created by Sándor Fehérvári on 2015.03.02..
//  Copyright (c) 2015 sandorfehervari. All rights reserved.
//

#ifndef __rulerDetect__autoruler__
#define __rulerDetect__autoruler__

#include <iostream>
#include "opencv2/core/core.hpp"

#define BLOCK_SIZE 20

using namespace cv;

class AutoRuler {
    
private:
    Mat* inputImage;
    void generateBlocksFromImage();
    Mat** imageBlocks;
    int samplesPerWidth, samplesPerHeight;
public:
    AutoRuler(Mat& input);
    void generateImage();
    Mat** getImageBlocks() {return this->imageBlocks;}
    int getSamplesPerWidth() {return this->samplesPerWidth;}
    int getSamplesPerHeight() {return this->samplesPerHeight;}
};

#endif /* defined(__rulerDetect__autoruler__) */

