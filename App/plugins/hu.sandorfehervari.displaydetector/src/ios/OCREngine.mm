//
//  OCREngine.m
//  Analog Display Reader
//
//  Created by Fehervari Sandor on 2015. 03. 29..
//
//

#include "OCREngine.h"
#import "OpenCVUtils.h"
#import <opencv2/opencv.hpp>
#import <TesseractOCR/TesseractOCR.h>

using namespace cv;

struct TesseractImpl {
    G8Tesseract *tesseract;
};

OCREngine::OCREngine(Mat& inputImage) {
    ocrEngine = new TesseractImpl();
    this->inputImage = &inputImage;
    ocrEngine->tesseract = [[G8Tesseract alloc] initWithLanguage:@"eng"];
    ocrEngine->tesseract.charWhitelist = @"0123456789";
    ocrEngine->tesseract.maximumRecognitionTime = 2.0;
    
    ocrEngine->tesseract.image = [OpenCVUtils UIImageFromCVMat:inputImage];
}

int OCREngine::getNumberFromImage(const cv::Rect &roi) {
    CGPoint point;
    point.x = roi.x;
    point.y = roi.y;
    
    CGSize size;
    size.height = roi.height;
    size.width = roi.width;
    
    CGRect rect;
    rect.origin = point;
    rect.size = size;
    
    CGRect rectOfInterest;
    rectOfInterest.origin = point;
    rectOfInterest.size = size;
    ocrEngine->tesseract.rect = rectOfInterest;
    [ocrEngine->tesseract recognize];
    NSString* recognizedText = [ocrEngine->tesseract recognizedText];
    NSLog(@"%@", recognizedText);
    return [recognizedText integerValue];
}
