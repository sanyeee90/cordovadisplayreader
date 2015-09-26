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

#include <iostream>
#include <ctime>

using namespace std;

void parseHoughCircles(Mat& input, Mat& output);
float calculateIndicatorPosition(vector<pair<Point, int> >& numberPoints, Point& indicatorLocation);

int numbers[] = { 60, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 800};

void preprocessImage() {
    
}

float readResultFromPFM(Mat& inputImage, const char* tessDataDir) {
#ifdef DEBUG
    clock_t begin = clock();
#endif
    Mat blurred;
    Mat hsvImage;
    Mat gray;
    Mat filtered_gray;
    
    cv::Size s = inputImage.size();
    
    if (isResizingRequired(s)) {
        resize(inputImage, inputImage, calculateOptimalSize(s));
    }

    
    GaussianBlur(inputImage, blurred, Size(0,0), 3);
    addWeighted(inputImage, 1.5, blurred, -0.5, 0, inputImage);
    
    cvtColor(inputImage, hsvImage, COLOR_BGR2HSV);
    cvtColor(inputImage, gray, COLOR_RGB2GRAY);
    
    // ---- INDICATOR, NUMBER_PLATE START ---- //
    Point indicatorPosition = extractIndicator(hsvImage);
    circle(inputImage, indicatorPosition, 10, Scalar(255,0,0));
    Mat bigestYellowBlob(hsvImage.size(), CV_8U, cvScalar(0));
    Rect numberPlatePlacement = extractNumberPlate(hsvImage, bigestYellowBlob);
    
    // ---- INDICATOR, NUMBER_PLATE END ---- //
    
    // ---- NUMBER FIELDS START ---- //
    gray.copyTo(filtered_gray, bigestYellowBlob);
    Mat binary(gray.size(), CV_8U, cvScalar(0));
    vector<Rect> boundRect;
    extractNumberFields(filtered_gray, numberPlatePlacement, binary, boundRect);

    //parseHoughCircles(binary,binary);
    // ---- NUMBER FIELDS END ---- //
    // ---- NUMBERS START ---- //
    //cv::Mat b = (cv::Mat_<uchar>(3,3) << 1,1,0,0,1,0,0,1,1);
    //threshold(filtered_gray, filtered_gray, 100, 180, THRESH_BINARY);
    bitwise_not(filtered_gray, filtered_gray);
    // ---- NUMBERS END ---- //
#ifdef DEBUG
    imshow("filtered_gray.png", filtered_gray);
#endif
    // BOUNDARY OF NUMBERS //
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(binary, contours, hierarchy, RETR_EXTERNAL,  CHAIN_APPROX_SIMPLE, Point(0, 0) );
    // BOUNDARY OF NUMBERS END //
    /// Approximate contours to polygons + get bounding rects and circles
    vector<pair<Point, int> > pointsWithNumbers(boundRect.size());
#ifdef DEBUG
    imshow("filtered_gray", filtered_gray);
#endif
    
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
        pointsWithNumbers[i] = pair<Point,int>(calculateCenterOfRectangle(boundRect[i]), number);
    }
#ifdef DEBUG
    imshow("thresh_grey", inputImage);
    imshow("inputImage", binary);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "elapsed time: " << elapsed_secs;
#endif
    //return 0;
    return calculateIndicatorPosition(pointsWithNumbers, indicatorPosition);
}

