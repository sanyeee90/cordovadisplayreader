//
//  IndicatorValueDetector.h
//  Analog Display Reader
//
//  Created by Fehervari Sandor on 2015. 03. 29..
//
//

#ifndef __Analog_Display_Reader__IndicatorValueDetector__
#define __Analog_Display_Reader__IndicatorValueDetector__

#include <opencv2/opencv.hpp>

std::vector<std::pair<cv::Point, int> > readValueIntervals(cv::Mat& binaryImage, cv::Mat& filtered_gray);
float calculateIndicatorPosition(std::vector<std::pair<cv::Point, int> > &numberPoints, cv::Point &indicatorLocation);
#endif /* defined(__Analog_Display_Reader__IndicatorValueDetector__) */
