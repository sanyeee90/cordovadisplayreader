//
//  line.cpp
//  rulerDetect
//
//  Created by Sándor Fehérvári on 2015.02.01..
//  Copyright (c) 2015 sandorfehervari. All rights reserved.
//

#include "line.h"
#include <iostream>

static const int LINE_START = 0;

pair<Point, Point> Line::getLinePoints(int width, int height) {
    float radianAngle = angle * CV_PI / 180.0;
    float m = tan(radianAngle);
    float n = startPoint.y - (tan (radianAngle) * startPoint.x );
    Point p1(LINE_START, LINE_START*m + n);
    Point p2(width, width*m + n);
    return pair<Point, Point>(p1,p2);
}

void Line::applyToImage(cv::Mat &inputImg, const Scalar& color) {
    pair<Point, Point> linePoints = getLinePoints(inputImg.cols, inputImg.rows);
    line(inputImg, linePoints.first, linePoints.second, color);
}

vector<int16_t> Line::getImageData(const cv::Mat &input) {
    pair<Point, Point> linePoints = getLinePoints(input.cols, input.rows);
    cv::LineIterator it(input, linePoints.first, linePoints.second, 8);
    vector<int16_t> buf;
    buf.resize(it.count);
    for(int i = 0; i < it.count; i++, ++it)
    {
        buf[i] = (const int16_t) it.ptr[i];
        std::cout<<buf[i] << endl;
    }
    return buf;
}

void Line::applyScalePoints(cv::Mat &inputImg, const int length) {
    pair<Point, Point> linePoints = getLinePoints(inputImg.cols, inputImg.rows);
    cv::LineIterator it(inputImg, linePoints.first, linePoints.second, 8);

    for(int i = 0; i < it.count; i++, ++it)
    {
        if(i%length==0)
        circle(inputImg, it.pos(), 5, Scalar(0,255,0));
    }
}