//
//  ScaleDetector.m
//  Analog Display Reader
//
//  Created by Sándor Fehérvári on 2015.02.12..
//
//
#import "preprocess_image.h"
#import "element_extraction.h"
#import "IndicatorValueDetector.h"
#import "constants.h"
#import "OCREngine.h"
#include "utils.h"

#import <opencv2/opencv.hpp>

#import "ScaleDetector.h"

@implementation ScaleDetector

int numbers[] = { 60, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 800};

- (cv::Mat)cvMatFromUIImage:(UIImage *)image
{
    CGColorSpaceRef colorSpace = CGImageGetColorSpace(image.CGImage);
    CGFloat cols = image.size.width;
    CGFloat rows = image.size.height;
    
    cv::Mat cvMat(rows, cols, CV_8UC4); // 8 bits per component, 4 channels (color channels + alpha)
    
    CGContextRef contextRef = CGBitmapContextCreate(cvMat.data,                 // Pointer to  data
                                                    cols,                       // Width of bitmap
                                                    rows,                       // Height of bitmap
                                                    8,                          // Bits per component
                                                    cvMat.step[0],              // Bytes per row
                                                    colorSpace,                 // Colorspace
                                                    kCGImageAlphaNoneSkipLast |
                                                    kCGBitmapByteOrderDefault); // Bitmap info flags
    
    CGContextDrawImage(contextRef, CGRectMake(0, 0, cols, rows), image.CGImage);
    CGContextRelease(contextRef);
    
    return cvMat;
}

-(float) applyDetectorAlgorithm:(NSString*) imgURL {
    float retValue = 0.0f;
    
    UIImage* image = [UIImage imageNamed:imgURL];
    cv::Mat imageToProcess;
    cv::Mat processed;
    cv::Mat hsvImage, gray;
    cv::Mat filtered_gray;
    
    imageToProcess = [self cvMatFromUIImage: image];
    
    sharpenImage(imageToProcess);
    
    cvtColor(imageToProcess, hsvImage, cv::COLOR_RGB2HSV);
    
    cvtColor(imageToProcess, gray, cv::COLOR_BGR2GRAY);
    
    // ---- INDICATOR, NUMBER_PLATE START ---- //
    
    cv::Point indicatorPosition = extractIndicator(hsvImage);
    
    cv::Mat bigestYellowBlob(hsvImage.size(), CV_8U);
    extractNumberPlate(hsvImage, bigestYellowBlob);
    
    // ---- INDICATOR, NUMBER_PLATE END ---- //
    
    // ---- NUMBER FIELDS START ---- //
    
    gray.copyTo(filtered_gray, bigestYellowBlob);
    
    cv::Mat binary(gray.size(), CV_8U, cv::Scalar(0));
    
    extractNumberFields(filtered_gray, binary);
    
    // ---- NUMBER FIELDS END ---- //
    
    // ---- NUMBERS START ---- //
    
    cv::Mat b = (cv::Mat_<uchar>(3,3) << 1,1,0,0,1,0,0,1,1);
    
    threshold(filtered_gray, filtered_gray, 110, 180, THRESH_BINARY);
    bitwise_not(filtered_gray, filtered_gray);
    erode(filtered_gray, filtered_gray, b);
    
    // ---- NUMBERS END ---- //

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    
    findContours(binary, contours, hierarchy, RETR_EXTERNAL,  CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
    
    vector<cv::Rect> boundRect( contours.size() );
    vector<vector<cv::Point> > contours_poly( contours.size() );
    
    vector<std::pair<cv::Point, int> > pointsWithNumbers(contours.size());
    
    UIImage* resultImg = [self UIImageFromCVMat:filtered_gray];
    
    
    OCREngine *ocr = [[OCREngine alloc] initWithImage:resultImg];
    
    for( int i = 0; i < contours.size(); i++ )
    {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );

        CGPoint point;
        point.x = boundRect[i].x;
        point.y = boundRect[i].y;
        
        CGSize size;
        size.height = boundRect[i].height;
        size.width = boundRect[i].width;
        
        CGRect *rect = new CGRect();
        rect->origin = point;
        rect->size = size;
        
        NSInteger number = [ocr getNumberFromImage: rect];
        
        NSLog(@"Read number: %d \n", number);
        
        pointsWithNumbers[i] = std::pair<cv::Point,int>(calculateCenterOfRectangle(boundRect[i]), numbers[i]);
        delete rect;
    }
    
    retValue = calculateIndicatorPosition(pointsWithNumbers, indicatorPosition);
    
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *filePath = [[paths objectAtIndex:0] stringByAppendingPathComponent:@"Image.png"];
    NSLog(filePath);
    // Save image.
    [UIImagePNGRepresentation(resultImg) writeToFile:filePath atomically:YES];
    
    return retValue;
}

-(UIImage *)UIImageFromCVMat:(cv::Mat)cvMat
{
    NSData *data = [NSData dataWithBytes:cvMat.data length:cvMat.elemSize()*cvMat.total()];
    CGColorSpaceRef colorSpace;
    
    if (cvMat.elemSize() == 1) {
        colorSpace = CGColorSpaceCreateDeviceGray();
    } else {
        colorSpace = CGColorSpaceCreateDeviceRGB();
    }
    
    CGDataProviderRef provider = CGDataProviderCreateWithCFData((__bridge CFDataRef)data);
    
    // Creating CGImage from cv::Mat
    CGImageRef imageRef = CGImageCreate(cvMat.cols,                                 //width
                                        cvMat.rows,                                 //height
                                        8,                                          //bits per component
                                        8 * cvMat.elemSize(),                       //bits per pixel
                                        cvMat.step[0],                            //bytesPerRow
                                        colorSpace,                                 //colorspace
                                        kCGImageAlphaNone|kCGBitmapByteOrderDefault,// bitmap info
                                        provider,                                   //CGDataProviderRef
                                        NULL,                                       //decode
                                        false,                                      //should interpolate
                                        kCGRenderingIntentDefault                   //intent
                                        );
    
    
    // Getting UIImage from CGImage
    UIImage *finalImage = [UIImage imageWithCGImage:imageRef];
    CGImageRelease(imageRef);
    CGDataProviderRelease(provider);
    CGColorSpaceRelease(colorSpace);
    
    return finalImage;
}


@end
