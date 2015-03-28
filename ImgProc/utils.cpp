//
//  utils.cpp
//  Csucsaramlasmero
//
//  Created by Fehervari Sandor on 2015. 03. 28..
//  Copyright (c) 2015. sandorfehervari. All rights reserved.
//

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "utils.h"

using namespace cv;
using namespace std;

Point calculateCenterOfRectangle(Rect rectangle) {
    Point center((rectangle.tl().x + rectangle.br().x) / 2, (rectangle.tl().y + rectangle.br().y) / 2 );
    return center;
}

bool compareByHeight(pair<Point, int> p1, pair<Point, int> p2){
    return (p1.first.y < p2.first.y);
}

void findBiggestBlob(cv::Mat &src, cv::Mat &dst){
    int largest_area=0;
    int largest_contour_index=0;
    Mat temp(src.rows,src.cols,CV_8UC1);
    Rect boundRect;
    src.copyTo(temp);
    
    vector<vector<Point>> contours; // storing contour
    vector<Vec4i> hierarchy;
    
    findContours( temp, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE );
    
    for( int i = 0; i< contours.size(); i++ ) // iterate
    {
        double a=contourArea( contours[i],false);  //Find the largest area of contour
        if(a>largest_area)
        {
            largest_area=a;
            largest_contour_index=i;
        }
        
    }
    
    boundRect = boundingRect(Mat(contours[largest_contour_index]));
    drawContours( dst, contours,largest_contour_index, Scalar(255), CV_FILLED, 8, hierarchy );
    rectangle( dst, boundRect.tl(), boundRect.br(), Scalar(255), CV_FILLED, 8, 0 );
    // Draw the largest contour
}
