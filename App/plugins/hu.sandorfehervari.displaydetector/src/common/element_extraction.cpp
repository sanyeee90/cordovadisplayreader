//
//  element_extraction.cpp
//  Csucsaramlasmero
//
//  Created by Fehervari Sandor on 2015. 03. 28..
//  Copyright (c) 2015. sandorfehervari. All rights reserved.
//

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "element_extraction.h"
#include "colour_based_extractor.h"
#include "constants.h"
#include "utils.h"

using namespace cv;
using namespace std;

Point extractIndicator(Mat& hsvInputImage) {
    Mat extracted, lower_red, upper_red, lines;
    ColourBasedExtractor lowerRed(Scalar(0,176,139), Scalar(4,243,181));
    lowerRed.ExtractColour(hsvInputImage, lower_red);
    ColourBasedExtractor upperRed(Scalar(168,160,138), Scalar(179,255,255));
    upperRed.ExtractColour(hsvInputImage, upper_red);
    bitwise_or(lower_red, upper_red, extracted);
    Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size(3,3));
    morphologyEx(extracted, extracted, MORPH_CLOSE, kernel);
    char name[40];

    cv::Mat b = (cv::Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,1);
    for(int i=0; i < 8; i++){
        erode(extracted, extracted, b);
    }
    Mat blob(extracted.size(), CV_8U);
    findBiggestBlob(extracted, blob);
    Canny(extracted, lines, 20, 20*3);
    
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(lines, contours, hierarchy, RETR_EXTERNAL,  CHAIN_APPROX_SIMPLE, Point(0, 0) );
    Mat temp(extracted.size(), CV_8U);
    if (contours.size() > 0) {
        Point mostLeftPoint(contours[0][0]);
        for (int i = 0; i<contours.size(); i++) {
            approxPolyDP(contours[i], contours[i], 2, true);
            for (int j = 0; j < contours[i].size(); j++) {
                if (mostLeftPoint.x > contours[i][j].x) {
                    mostLeftPoint = contours[i][j];
                }
            }
        }
        hsvInputImage.copyTo(temp);
        circle(temp, mostLeftPoint, 4, Scalar(255));
        return mostLeftPoint;
    }
    Point pt;
    return pt;
}


void extractNumberPlate(cv::Mat& hsvInputImage, cv::Mat& dst) {
    Mat extractedColor(hsvInputImage.size(), CV_8U);
    ColourBasedExtractor colourExtractor(YELLOW_RANGE_START, YELLOW_RANGE_END);
    
    colourExtractor.ExtractColour(hsvInputImage, extractedColor);
    findBiggestBlob(extractedColor, dst, true);
}

void extractNumberFields(cv::Mat& grayInputImage, cv::Mat& dst) {
    int allContours = -1;
    int thickness = 2;
    Mat output;
    threshold(grayInputImage, output, 90, 255, THRESH_BINARY);
    cv::Mat b = (cv::Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,1);

        erode(output, output, b);

    GaussianBlur(grayInputImage, grayInputImage, Size(3,3), 3);
    Mat masked;
    grayInputImage.copyTo(masked);
    imshow("new", masked);
    MSER ms(7,100,1000);
    vector<vector <Point> > regions;
    ms(masked, regions);
    drawContours(dst, regions, allContours, Scalar::all(255), thickness);
    bitwise_not(dst, dst);
    
    int morph_size = 1;
    Mat element = getStructuringElement( MORPH_ELLIPSE, Size( 2*morph_size + 1, 2*morph_size+1 ) );
    morphologyEx( dst, dst, MORPH_OPEN, element, Point(-1,-1), 1 );
    morphologyEx( dst, dst, MORPH_CLOSE, element, Point(-1,-1), 1 );
    
    bitwise_not(dst, dst);
    imshow("dst", dst);
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
    size_t ncomp = contours.size();
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
