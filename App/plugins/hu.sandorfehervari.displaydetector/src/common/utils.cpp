//
//  utils.cpp
//  Csucsaramlasmero
//
//  Created by Fehervari Sandor on 2015. 03. 28..
//  Copyright (c) 2015. sandorfehervari. All rights reserved.
//

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "utils.h"
#include "constants.h"

using namespace cv;
using namespace std;

Point calculateCenterOfRectangle(Rect rectangle) {
    Point center((rectangle.tl().x + rectangle.br().x) / 2, (rectangle.tl().y + rectangle.br().y) / 2 );
    return center;
}

bool compareByHeight(pair<Point, int> p1, pair<Point, int> p2){
    return (p1.first.y < p2.first.y);
}

int calculateBoundaryExtra(const Rect& rectToCalculate, const Size& imgDimension) {
    int proposedExtraWidth = rectToCalculate.width * NUMBER_FIELD_EXTRA_PERCENT;
    int proposedExtraHeight = rectToCalculate.height * NUMBER_FIELD_EXTRA_PERCENT;
    int proposedExtra = min(proposedExtraHeight, proposedExtraWidth);
    
    if ((rectToCalculate.tl().x - proposedExtra >= 0) && (rectToCalculate.tl().y - proposedExtra >= 0) && (rectToCalculate.br().x + proposedExtra <= imgDimension.width) && (rectToCalculate.br().y + proposedExtra <= imgDimension.height)) {
        return proposedExtra;
    } else {
        int maximalExtraWidth = min(rectToCalculate.tl().x, imgDimension.width-rectToCalculate.br().x);
        int maximalExtraHeight = min(rectToCalculate.tl().y, imgDimension.height- rectToCalculate.br().y);
        return min(maximalExtraHeight, maximalExtraWidth);
        
    }
}

Rect findBiggestBlob(cv::Mat &src,bool needExtra){
    int largest_area=0;
    int largest_contour_index=0;
    Mat temp(src.rows,src.cols,CV_8UC1);
    Rect boundRect;
    src.copyTo(temp);
    
    vector<vector<Point> > contours; // storing contour
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
    if (contours.size() > 0) {
        boundRect = boundingRect(Mat(contours[largest_contour_index]));
        if (needExtra) {
            int boundRectExtra = calculateBoundaryExtra(boundRect, src.size());
            boundRect.x-=boundRectExtra;
            boundRect.y-=boundRectExtra;
            boundRect.width+=boundRectExtra;
            boundRect.height+=boundRectExtra;
        }
    }
    return boundRect;

}

bool isResizingRequired(cv::Size& imageSize) {
    if (imageSize.width > OPTIMAL_IMAGE_SIZE || imageSize.height > OPTIMAL_IMAGE_SIZE) {
        return true;
    }
    return false;
}

cv::Size calculateOptimalSize(const cv::Size& imageSize) {
    Size retValue;
    float aspectRatio = 0.0f;
    if (imageSize.height == 0 || imageSize.width == 0) {
        return retValue;
    }
    // landscape case
    if (imageSize.width > imageSize.height) {
        aspectRatio = imageSize.width / (float)imageSize.height;
        retValue.width = OPTIMAL_IMAGE_SIZE;
        retValue.height = OPTIMAL_IMAGE_SIZE / aspectRatio;
    //portrait and square cases
    } else {
        aspectRatio =imageSize.height / (float)imageSize.width;
        retValue.height = OPTIMAL_IMAGE_SIZE;
        retValue.width = OPTIMAL_IMAGE_SIZE / aspectRatio;
    }
    return retValue;
    
}

