//
//  OCREngine.cpp
//  Csucsaramlasmero
//
//  Created by Fehervari Sandor on 2015. 03. 28..
//  Copyright (c) 2015. sandorfehervari. All rights reserved.
//

#include "OCREngine.h"

using namespace cv;

OCREngine::OCREngine(Mat& inputmap) {
    this->inputImage = &inputmap;
    ocrEngine.Init("/", "eng", tesseract::OEM_DEFAULT);
    ocrEngine.SetVariable("tessedit_char_whitelist", "0123456789");
    ocrEngine.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
}

int OCREngine::getNumberFromImage(const Rect& roi) {
    int number;
    Mat imgToDetect = this->inputImage->operator()(roi);
    ocrEngine.TesseractRect( imgToDetect.data, 1, imgToDetect.step1(), 0, 0, imgToDetect.cols, imgToDetect.rows);
    char* out = ocrEngine.GetUTF8Text();
    sscanf(out, "%d", &number);
    return number;
}
