//
//  OCREngine.cpp
//  Csucsaramlasmero
//
//  Created by Fehervari Sandor on 2015. 03. 28..
//  Copyright (c) 2015. sandorfehervari. All rights reserved.
//

#include "OCREngine.h"

using namespace cv;

OCREngine::OCREngine() {
    ocrEngine.Init(NULL, "eng", tesseract::OEM_DEFAULT);
    ocrEngine.SetVariable("tessedit_char_whitelist", "0123456789");
    ocrEngine.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
}

int OCREngine::getNumberFromImage(Mat& inputImage) {
    int number;
    ocrEngine.TesseractRect( inputImage.data, 1, inputImage.step1(), 0, 0, inputImage.cols, inputImage.rows);
    char* out = ocrEngine.GetUTF8Text();
    sscanf(out, "%d", &number);
    return number;
}