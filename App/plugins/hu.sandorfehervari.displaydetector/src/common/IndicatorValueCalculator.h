//
//  IndicatorValueDetector.h
//  Analog Display Reader
//
//  Created by Fehervari Sandor on 2015. 03. 29..
//
//

#ifndef __Analog_Display_Reader__IndicatorValueDetector__
#define __Analog_Display_Reader__IndicatorValueDetector__

#import <opencv2/opencv.hpp>

float calculateIndicatorPosition(cv::vector<std::pair<cv::Point, int>>& numberPoints, cv::Point& indicatorLocation);
#endif /* defined(__Analog_Display_Reader__IndicatorValueDetector__) */
