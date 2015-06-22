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

#include <tesseract/baseapi.h>

#include <iostream>
#include <ctime>

using namespace std;

void parseHoughCircles(Mat& input, Mat& output);
float calculateIndicatorPosition(vector<pair<Point, int>>& numberPoints, Point& indicatorLocation);

int numbers[] = { 60, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 800};

float readResultFromPFM(Mat& inputImage) {
#ifdef DEBUG
    clock_t begin = clock();
#endif
    Mat blurred;
    Mat hsvImage;
    Mat gray;
    Mat filtered_gray;
    
    GaussianBlur(inputImage, blurred, Size(0,0), 3);
    addWeighted(inputImage, 1.5, blurred, -0.5, 0, inputImage);
    
    cvtColor(inputImage, hsvImage, COLOR_BGR2HSV);
    cvtColor(inputImage, gray, COLOR_RGB2GRAY);
    
    // ---- INDICATOR, NUMBER_PLATE START ---- //
    Point indicatorPosition = extractIndicator(hsvImage);
    
    Mat bigestYellowBlob(hsvImage.size(), CV_8U, cvScalar(0));
    extractNumberPlate(hsvImage, bigestYellowBlob);
    
    // ---- INDICATOR, NUMBER_PLATE END ---- //
    
    // ---- NUMBER FIELDS START ---- //
    gray.copyTo(filtered_gray, bigestYellowBlob);
#ifdef DEBUG
    imshow("filtered_gray.png", bigestYellowBlob);
#endif
    Mat binary(gray.size(), CV_8U, cvScalar(0));
    extractNumberFields(filtered_gray, binary);
#ifdef DEBUG
    imshow("filtered_gray.png", binary);
#endif
    parseHoughCircles(binary,binary);
    // ---- NUMBER FIELDS END ---- //
    // ---- NUMBERS START ---- //
    cv::Mat b = (cv::Mat_<uchar>(3,3) << 1,1,0,0,1,0,0,1,1);
    threshold(filtered_gray, filtered_gray, 100, 180, THRESH_BINARY);
    bitwise_not(filtered_gray, filtered_gray);
    // ---- NUMBERS END ---- //
    
    // BOUNDARY OF NUMBERS //
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(binary, contours, hierarchy, RETR_EXTERNAL,  CHAIN_APPROX_SIMPLE, Point(0, 0) );
    // BOUNDARY OF NUMBERS END //
    vector<Rect> boundRect( contours.size() );
    /// Approximate contours to polygons + get bounding rects and circles
    vector<vector<Point> > contours_poly( contours.size() );
    vector<pair<Point, int>> pointsWithNumbers(contours.size());
#ifdef DEBUG
    imshow("filtered_gray", filtered_gray);
#endif
    
    OCREngine ocr(filtered_gray);
    for( int i = 0; i < contours.size(); i++ )
    {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
        char szamok[30];
#ifdef DEBUG
        rectangle(inputImage, boundRect[i].tl(), boundRect[i].br(), Scalar(255,0,255));
#endif

        
        int number = ocr.getNumberFromImage(boundRect[i]);
        sprintf(szamok, "%d\n", number);
        cout << szamok;
        pointsWithNumbers[i] = pair<Point,int>(calculateCenterOfRectangle(boundRect[i]), numbers[i]);
    }
#ifdef DEBUG
    imshow("thresh_grey", inputImage);
    imshow("inputImage", binary);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "elapsed time: " << elapsed_secs;
#endif
    return calculateIndicatorPosition(pointsWithNumbers, indicatorPosition);
}

float calculateIndicatorPosition(vector<pair<Point, int>>& numberPoints, Point& indicatorLocation) {
    sort(numberPoints.begin(), numberPoints.end(), compareByHeight);
    int selectedIndex = 0;
    for (int i = 0; i < numberPoints.size()-1; i++) {
        if(numberPoints[i].first.y < indicatorLocation.y && numberPoints[i+1].first.y > indicatorLocation.y) {
            selectedIndex=i;
        }
    }
    float startInterval, endInterval;
    startInterval = numberPoints[selectedIndex].first.y;
    endInterval = numberPoints[selectedIndex + 1].first.y;
    
    float normalized = endInterval - startInterval;
    
    float percentage = (indicatorLocation.y-startInterval) / normalized;
    
    float resultAmount = (numberPoints[selectedIndex+1].second - numberPoints[selectedIndex].second) * percentage + numberPoints[selectedIndex].second;
    cout << "interval: " << numberPoints[selectedIndex+1].second << ", min: " << numberPoints[selectedIndex].second << endl;
    
    cout << "resultAmount: " << resultAmount;
    
    
    return resultAmount;
}

void parseHoughCircles(Mat& input, Mat& mask) {
    vector<Vec3f> circles;
    Mat coloredInput;
    input.copyTo(coloredInput);
    cvtColor(coloredInput, coloredInput, CV_GRAY2BGR);
    Mat temp;
    distanceTransform(input, temp, CV_DIST_L2, 3);
    normalize(temp, temp, 0, 1., cv::NORM_MINMAX);
    threshold(temp, temp, .5, 1., CV_THRESH_BINARY);
    cv::Mat dist_8u;
    temp.convertTo(dist_8u, CV_8U);
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    cv::Mat markers = cv::Mat::zeros(temp.size(), CV_32SC1);
    int ncomp = contours.size();
    for (int i = 0; i < contours.size(); i++)
        cv::drawContours(markers, contours, i, cv::Scalar::all(i+1), -1);
    circle(markers, cv::Point(5,5), 3, CV_RGB(255,255,255), -1);
    watershed(coloredInput, markers);
    
    // Generate random colors
    std::vector<cv::Vec3b> colors;
    for (int i = 0; i < ncomp; i++)
    {
        int b = cv::theRNG().uniform(0, 255);
        int g = cv::theRNG().uniform(0, 255);
        int r = cv::theRNG().uniform(0, 255);
        
        colors.push_back(cv::Vec3b((uchar)b, (uchar)g, (uchar)r));
    }
    
    // Create the result image
    cv::Mat dst = cv::Mat::zeros(markers.size(), CV_8UC3);
    
    // Fill labeled objects with random colors
    for (int i = 0; i < markers.rows; i++)
    {
        for (int j = 0; j < markers.cols; j++)
        {
            int index = markers.at<int>(i,j);
            if (index > 0 && index <= ncomp)
                dst.at<cv::Vec3b>(i,j) = colors[index-1];
            else
                dst.at<cv::Vec3b>(i,j) = cv::Vec3b(0,0,0);
        }
    }
    
}

