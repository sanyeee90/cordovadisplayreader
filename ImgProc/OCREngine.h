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
#include <tesseract/baseapi.h>
#include <opencv2/core/core.hpp>

class OCREngine {
private:
    tesseract::TessBaseAPI ocrEngine;
    
public:
    OCREngine();
    int getNumberFromImage(cv::Mat& inputImage);
    
};

#endif /* defined(__Csucsaramlasmero__OCREngine__) */
