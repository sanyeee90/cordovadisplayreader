//
//  OCREngine.h
//  Csucsaramlasmero
//
//  Created by Fehervari Sandor on 2015. 03. 28..
//  Copyright (c) 2015. sandorfehervari. All rights reserved.
//

#ifndef __Csucsaramlasmero__OCREngine__
#define __Csucsaramlasmero__OCREngine__

#include <stdio.h>
#include <opencv2/core/core.hpp>

struct TesseractImpl;

class OCREngine {
private:
    cv::Mat* inputImage;
    TesseractImpl* ocrEngine;
    char* tessDataDir;
public:
    ~OCREngine();
    OCREngine(cv::Mat&, const char* tessDataDir);
    int getNumberFromImage(const cv::Rect& roi);
    
};

#endif /* defined(__Csucsaramlasmero__OCREngine__) */
