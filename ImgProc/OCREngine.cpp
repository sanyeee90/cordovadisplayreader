//
//  OCREngine.cpp
//  Csucsaramlasmero
//
//  Created by Fehervari Sandor on 2015. 03. 28..
//  Copyright (c) 2015. sandorfehervari. All rights reserved.
//

#include "OCREngine.h"
#include <tesseract/baseapi.h>
using namespace cv;

struct TesseractImpl {
    tesseract::TessBaseAPI *tesseractOCR;
};

OCREngine::OCREngine(Mat& inputmap, const char* tessDataDict) {
    ocrEngine = new TesseractImpl();
    ocrEngine->tesseractOCR = new tesseract::TessBaseAPI();
    this->inputImage = &inputmap;
    ocrEngine->tesseractOCR->Init("/", "eng", tesseract::OEM_DEFAULT);
    ocrEngine->tesseractOCR->SetVariable("tessedit_char_whitelist", "0123456789");
    ocrEngine->tesseractOCR->SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
}

OCREngine::~OCREngine() {
    if (ocrEngine) {
        delete ocrEngine->tesseractOCR;
    }

    delete ocrEngine;
}

int OCREngine::getNumberFromImage(const Rect& roi) {
    int number;
    Mat imgToDetect = this->inputImage->operator()(roi);
    ocrEngine->tesseractOCR->TesseractRect( imgToDetect.data, 1, imgToDetect.step1(), 0, 0, imgToDetect.cols, imgToDetect.rows);
    char* out = ocrEngine->tesseractOCR->GetUTF8Text();
    sscanf(out, "%d", &number);
    return number;
}
