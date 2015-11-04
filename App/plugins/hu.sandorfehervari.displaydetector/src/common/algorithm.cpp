//
//  algorithm.cpp
//  Csucsaramlasmero
//
//  Created by Fehervari Sandor on 2015. 06. 21..
//  Copyright (c) 2015. sandorfehervari. All rights reserved.
//

#include "algorithm.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/features2d/features2d.hpp>

#include "element_extraction.h"
#include "OCREngine.h"
#include "utils.h"
#include "constants.h"
#include "IndicatorValueDetector.h"
#include "EmptyImageException.h"
#include <iostream>
#include <ctime>

using namespace std;

Rect getInterestedRegionAroundTheIndicator(Rect& indicatorRect, const Size& originalImageSize) {
    int interestedStartYPoint = indicatorRect.tl().y - indicatorRect.height * AREA_MULTIPLER;
    interestedStartYPoint = interestedStartYPoint > 0 ? interestedStartYPoint : 0;
    int maximumHeightSupportedByImage = originalImageSize.height - interestedStartYPoint;
    int interestedHeight = indicatorRect.height * (AREA_MULTIPLER * 2 + 1);
    if (maximumHeightSupportedByImage < interestedHeight) {
        interestedHeight = maximumHeightSupportedByImage;
    }
    return Rect(0, interestedStartYPoint, originalImageSize.width, interestedHeight);
}

PFMResultDetector::PFMResultDetector(Mat& input, const char* tessDataDir) {
    Size originalSize = input.size();
    if (isResizingRequired(originalSize)) {
        resize(input, inputImage, calculateOptimalSize(originalSize));
    } else {
        inputImage = input.clone();
    }
    this->tessDataDir = tessDataDir;
    cvtColor(inputImage, hsvImage, COLOR_BGR2HSV);
}

void PFMResultDetector::detectIndicatorOnOriginalImage() {
    indicatorRect = extractIndicator(hsvImage);
    Rect interestedRegion = getInterestedRegionAroundTheIndicator(indicatorRect, inputImage.size());
    //Rect roiAroundIndicator(indicatorRect);
    indicatorRect.y = indicatorRect.y-interestedRegion.tl().y;
    hsvImage = hsvImage(interestedRegion);
    inputImage = inputImage(interestedRegion);
    //imwrite("extracted", inputImage);
    cvtColor(inputImage, grayImage, COLOR_BGR2GRAY);
}

void PFMResultDetector::detectNumberFields() {
    Mat numberPlateMask = cv::Mat::zeros(hsvImage.size(), CV_8U);
    Rect numberPlateArea = extractNumberPlate(hsvImage, numberPlateMask);

    // ---- NUMBER FIELDS START ---- //
    grayImage.copyTo(filtered_gray, numberPlateMask);
    extractNumberFields(filtered_gray, numberPlateArea, boundRect);
    transformCoordinatesWithBaseline(numberPlateArea, boundRect);
    bitwise_not(filtered_gray, filtered_gray);
}

void PFMResultDetector::readNumbersFromImage() {
    OCREngine ocr(filtered_gray, tessDataDir);
    for( int i = 0; i < boundRect.size(); i++ )
    {
        if (boundRect[i].height < indicatorRect.height) {
            rectangle(inputImage, boundRect[i], Scalar(255,0,0));
            int number = ocr.getNumberFromImage(boundRect[i]);
            pointsWithNumbers.push_back(pair<Point,int>(calculateCenterOfRectangle(boundRect[i]), number));
        }
    }
    
}

float PFMResultDetector::runAlgorithm() {
    try {
        detectIndicatorOnOriginalImage();

        detectNumberFields();
        //imshow("hsvimage", hsvImage);
        //imshow("indicatorOnOriginal", filtered_gray);
        readNumbersFromImage();
        Point updatedIndicatorPosition = calculateCenterOfRectangle(indicatorRect);
        float result = calculateIndicatorPosition(pointsWithNumbers, updatedIndicatorPosition);
        if (result < 0) {
            return ERROR_INVALID_RESULT;
        }
        return result;
    } catch (AlgorithmException ex) {
        return ex.getErrorCode();
    }

}