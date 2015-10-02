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
#include "opencv2/highgui/highgui.hpp"

#include "element_extraction.h"
#include "OCREngine.h"
#include "utils.h"
#include "constants.h"
#include <iostream>
#include <ctime>

using namespace std;

void parseHoughCircles(Mat& input, Mat& output);
float calculateIndicatorPosition(vector<pair<Point, int> >& numberPoints, Point& indicatorLocation);

int numbers[] = { 60, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 800};

void preprocessImage() {
    
}



float readResultFromPFM(Mat& input, const char* tessDataDir) {
#ifdef DEBUG
    clock_t begin = clock();
#endif
    Mat blurred;
    Mat hsvImage;
    Mat gray;
    Mat filtered_gray;
    Mat inputImage = input.clone();
    cv::Size s = inputImage.size();
    
    if (isResizingRequired(s)) {
        resize(inputImage, inputImage, calculateOptimalSize(s));
    }

    
    GaussianBlur(inputImage, blurred, Size(0,0), 3);
    addWeighted(inputImage, 1.5, blurred, -0.5, 0, inputImage);
    
    cvtColor(inputImage, hsvImage, COLOR_BGR2HSV);
    cvtColor(inputImage, gray, COLOR_RGB2GRAY);
    
    // ---- INDICATOR, NUMBER_PLATE START ---- //
    Rect indicatorRect = extractIndicator(hsvImage);
    Point indicatorPosition = calculateCenterOfRectangle(indicatorRect);
    
    // ---- INDICATOR, NUMBER_PLATE END ---- //
    
    
    Rect roiAroundIndicator(0, indicatorRect.tl().y-indicatorRect.height*2, inputImage.size().width, indicatorRect.height*5);
    Mat interestedImageAroundIndicatorHSV = hsvImage(roiAroundIndicator);
    Mat interestedImageAroundIndicatorGray = gray(roiAroundIndicator);
    Point updatedIndicatorPosition(0,roiAroundIndicator.height/2);
    
    
    Mat interestedAreaNumberPlateMat(interestedImageAroundIndicatorHSV.size(), CV_8U);
    
    Rect interestedNumberPlate = extractNumberPlate(interestedImageAroundIndicatorHSV, interestedAreaNumberPlateMat);

    // ---- NUMBER FIELDS START ---- //
    interestedImageAroundIndicatorGray.copyTo(filtered_gray, interestedAreaNumberPlateMat);
    
    vector<Rect> boundRect;
    extractNumberFields(filtered_gray, interestedNumberPlate, boundRect);
    transformCoordinatesWithBaseline(interestedNumberPlate, boundRect);
    imshow("orig", interestedImageAroundIndicatorHSV);
    
    circle(inputImage, indicatorPosition, 5, Scalar(0,255,0));
    
    bitwise_not(filtered_gray, filtered_gray);
    // ---- NUMBERS END ---- //
#ifdef DEBUG
    imshow("filtered_gray.png", filtered_gray);
#endif
    // BOUNDARY OF NUMBERS //
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    // BOUNDARY OF NUMBERS END //
    /// Approximate contours to polygons + get bounding rects and circles
#ifdef DEBUG
    imshow("filtered_gray", filtered_gray);
#endif
    vector<pair<Point, int> > pointsWithNumbers;
    if (boundRect.empty()) {
        return NUMBERS_EMPTY_RESULT;
    }
    
    OCREngine ocr(filtered_gray, tessDataDir);
    for( int i = 0; i < boundRect.size(); i++ )
    {
        char szamok[30];
#ifdef DEBUG
        rectangle(inputImage, boundRect[i].tl(), boundRect[i].br(), Scalar(255,0,255));
#endif
        int number = ocr.getNumberFromImage(boundRect[i]);
        sprintf(szamok, "%d\n", number);
        cout << szamok;
        pointsWithNumbers.push_back(pair<Point,int>(calculateCenterOfRectangle(boundRect[i]), number));
    }
#ifdef DEBUG
    imshow("thresh_grey", inputImage);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "elapsed time: " << elapsed_secs;
#endif
    //return 0;
    inputImage.release();
    return calculateIndicatorPosition(pointsWithNumbers, updatedIndicatorPosition);
}

