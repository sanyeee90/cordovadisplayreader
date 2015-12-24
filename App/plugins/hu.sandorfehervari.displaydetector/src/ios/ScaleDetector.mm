//
//  ScaleDetector.m
//  Analog Display Reader
//
//  Created by Sándor Fehérvári on 2015.02.12..
//
//
#include "algorithm.h"
#import <opencv2/opencv.hpp>
#import "OpenCVUtils.h"
#import "ScaleDetector.h"

@implementation ScaleDetector

-(float) applyDetectorAlgorithm:(UIImage*) image {
    float retValue = 0.0f;
    
    cv::Mat imageToProcess = [OpenCVUtils cvMatFromUIImage:image];
    PFMResultDetector pfm(imageToProcess);
    
    retValue = pfm.runAlgorithm();
    return retValue;
}


@end
