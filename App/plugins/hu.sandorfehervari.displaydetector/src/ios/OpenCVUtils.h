//
//  OpenCVUtils.h
//  Analog Display Reader
//
//  Created by Fehervari Sandor on 2015. 06. 22..
//
//
#import <opencv2/opencv.hpp>

@interface OpenCVUtils : NSObject

+ (cv::Mat)cvMatFromUIImage:(UIImage *)image;
+ (UIImage *)UIImageFromCVMat:(cv::Mat)cvMat;

@end

