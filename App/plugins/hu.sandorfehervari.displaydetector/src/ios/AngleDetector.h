//
//  AngleDetector.h
//  Analog Display Reader
//
//  Created by Sándor Fehérvári on 2015.02.12..
//
//

#ifndef __Analog_Display_Reader__AngleDetector__
#define __Analog_Display_Reader__AngleDetector__

#include <iostream>
#include "opencv2/opencv.hpp"

#define OUT_OF_ANGLE -1;

class AngleDetector {
private:
    float detected_angle;
    cv::Mat imageData;
    cv::Mat outputData;
    std::vector<uchar> imageDataAtAngle;
    void psdt();
    
public:
    AngleDetector(cv::Mat& inputImage) {inputImage.copyTo(imageData);}
    float getAngleValue();
    void getImageDataAtAngle(std::vector<int16_t>& inputVector );
    cv::Mat getOutputData() { return this->outputData; }
    cv::Mat getInputData() { return this->imageData; }
};


#endif /* defined(__Analog_Display_Reader__AngleDetector__) */
