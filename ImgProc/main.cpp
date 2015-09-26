#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/features2d/features2d.hpp>
#include "opencv2/highgui/highgui.hpp"

#include "element_extraction.h"
#include "OCREngine.h"
#include "utils.h"

#include <tesseract/baseapi.h>

#include <iostream>
#include "algorithm.h"

int main_range() {
    Mat box_amazon = imread("img/pfm20_amazon.jpg");
    Mat box_straight = imread("img/pfm20_straight.png");
    Mat box_shapred = imread("img/pfm20_sharped.png");
    Mat box_lowres = imread("img/pfm20.png");
    Mat box_straight_nonsharp = imread("img/pfm20det_straight.png");
    
    Mat box_amazon_hsv, box_straight_hsv,box_shapred_hsv,box_lowres_hsv,box_straight_nonsharp_hsv;
    
    
    cvtColor(box_amazon, box_amazon_hsv, COLOR_BGR2HSV);
    cvtColor(box_straight, box_straight_hsv, COLOR_BGR2HSV);
    cvtColor(box_shapred, box_shapred_hsv, COLOR_BGR2HSV);
    cvtColor(box_lowres, box_lowres_hsv, COLOR_BGR2HSV);
    cvtColor(box_straight_nonsharp, box_straight_nonsharp_hsv, COLOR_BGR2HSV);
    
    Mat yellow_amazon(box_amazon.size(), CV_8U, cvScalar(0));
    Mat straight(box_straight.size(), CV_8U, cvScalar(0));
    Mat sharped(box_shapred.size(), CV_8U, cvScalar(0));
    Mat lowres(box_lowres.size(), CV_8U, cvScalar(0));
    Mat straight_nonsharp(box_straight_nonsharp.size(), CV_8U, cvScalar(0));
    
    extractNumberPlate(box_amazon_hsv, yellow_amazon);
    extractNumberPlate(box_straight_hsv, straight);
    extractNumberPlate(box_shapred_hsv, sharped);
    extractNumberPlate(box_lowres_hsv, lowres);
    extractNumberPlate(box_straight_nonsharp_hsv, straight_nonsharp);
    Mat temp(straight.size(), CV_8U, cvScalar(0));
    box_straight.copyTo(temp, straight);
    imshow("amazonakurvaanyad", temp);
    imshow("straight", straight);
    imshow("sharped", sharped);
    imshow("lowres", lowres);
    imshow("straight_nonsharp", straight_nonsharp);
    
    /*extractIndicator(box_amazon_hsv, "pfm20_amazon.jpg");
    extractIndicator(box_straight_hsv, "pfm20_straight.png");
    extractIndicator(box_shapred_hsv, "pfm20_sharped.png");
    extractIndicator(box_lowres_hsv, "pfm20.png");
    extractIndicator(box_straight_nonsharp_hsv, "pfm20det_straight.png");*/
    waitKey(0);
    return 0;
}



int main(){
    //Mat box = imread("img/pfm20det.png");
    Mat box = imread("img/IMG_1322.png");
    float result = readResultFromPFM(box);
    printf("result: %f\n", result);
    waitKey(0);

    return 0;


}

