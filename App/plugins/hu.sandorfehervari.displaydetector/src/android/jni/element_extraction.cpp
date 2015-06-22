//
//  element_extraction.cpp
//  Csucsaramlasmero
//
//  Created by Fehervari Sandor on 2015. 03. 28..
//  Copyright (c) 2015. sandorfehervari. All rights reserved.
//

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>


#include "element_extraction.h"
#include "colour_based_extractor.h"
#include "constants.h"
#include "utils.h"

#include <iostream>

using namespace cv;
using namespace std;


Point extractIndicator(Mat& hsvInputImage) {
    Mat extracted, lower_red, upper_red, lines;

    inRange(hsvInputImage, Scalar(0,176,139), Scalar(4,243,181), lower_red);
    inRange(hsvInputImage, Scalar(168,160,138), Scalar(179,255,255), upper_red);
    
    bitwise_or(lower_red, upper_red, extracted);
    
    Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size(3,3));
    
    morphologyEx(extracted, extracted, MORPH_CLOSE, kernel);
    
    cv::Mat b = (cv::Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,1);
    for(int i=0; i < 3; i++){
        erode(extracted, extracted, b);
    }

    Mat blob(extracted.size(), CV_8U);
    
    findBiggestBlob(extracted, blob);

    Canny(extracted, lines, 20, 20*3);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(lines, contours, hierarchy, RETR_EXTERNAL,  CHAIN_APPROX_SIMPLE, Point(0, 0) );
    Mat temp(extracted.size(), CV_8U);
    Point mostLeftPoint(0,0);
    
    if (contours.size() > 0) {
        mostLeftPoint = contours[0][0];
        for (int i = 0; i<contours.size(); i++) {
            approxPolyDP(contours[i], contours[i], 2, true);
            for (int j = 0; j < contours[i].size(); j++) {
                if (mostLeftPoint.x > contours[i][j].x) {
                    mostLeftPoint = contours[i][j];
                }
            }
        }
    }
    
    return mostLeftPoint;
}


void extractNumberPlate(cv::Mat& hsvInputImage, cv::Mat& dst) {
    Mat extractedColor(hsvInputImage.size(), CV_8U);
    
    inRange(hsvInputImage,YELLOW_RANGE_START,YELLOW_RANGE_END, extractedColor);
    findBiggestBlob(extractedColor, dst);
    
}

void extractNumberFields(cv::Mat& grayInputImage, cv::Mat& dst) {
    int allContours = -1;
    int thickness = 2;
    MSER ms(9,100,1000);
    vector<vector <Point> > regions;
    ms(grayInputImage, regions);
    drawContours(dst, regions, allContours, Scalar::all(255), thickness);
    bitwise_not(dst, dst);
    
    int morph_size = 1;
    Mat element = getStructuringElement( MORPH_ELLIPSE, Size( 2*morph_size + 1, 2*morph_size+1 ) );
    morphologyEx( dst, dst, MORPH_OPEN, element, Point(-1,-1), 1 );
    morphologyEx( dst, dst, MORPH_CLOSE, element, Point(-1,-1), 1 );
    
    bitwise_not(dst, dst);
}
