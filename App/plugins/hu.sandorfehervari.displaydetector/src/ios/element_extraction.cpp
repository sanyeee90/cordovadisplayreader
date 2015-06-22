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


int extractIndicator(Mat& hsvInputImage, Point& indicatorPosition) {
    Mat extracted, lines;
    
    ColourBasedExtractor colourExtractor(Scalar(0,90,130), Scalar(12,255,241));
    
    colourExtractor.ExtractColour(hsvInputImage, extracted);
    
    cv::Mat b = (cv::Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,1);
    for(int i=0; i < 8; i++){
        erode(extracted, extracted, b);
    }
    
    Mat blob(extracted.size(), CV_8U);
    findBiggestBlob(extracted, blob);
    
    Canny(extracted, lines, 20, 20*3);
    
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(lines, contours, hierarchy, RETR_EXTERNAL,  CHAIN_APPROX_SIMPLE, Point(0, 0) );
    Mat temp(extracted.size(), CV_8U);
    if (contours.size()==0 || contours[0].size() == 0) {
        return INDICATOR_DETECTION_FAILED;
    }
    indicatorPosition = contours[0][0];
    for (int i = 0; i<contours.size(); i++) {
        approxPolyDP(contours[i], contours[i], 2, true);
        for (int j = 0; j < contours[i].size(); j++) {
            if (indicatorPosition.x > contours[i][j].x) {
                indicatorPosition = contours[i][j];
            }
        }
    }
    
    return COMMON_SUCCESS;
}


int extractNumberPlate(cv::Mat& hsvInputImage, cv::Mat& dst) {
    Mat extractedColor(hsvInputImage.size(), CV_8U);
    ColourBasedExtractor colourExtractor(YELLOW_RANGE_START, YELLOW_RANGE_END);
    
    colourExtractor.ExtractColour(hsvInputImage, extractedColor);
    return findBiggestBlob(extractedColor, dst);
    
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
