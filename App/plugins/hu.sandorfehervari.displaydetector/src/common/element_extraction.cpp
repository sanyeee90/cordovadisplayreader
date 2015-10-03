//
//  element_extraction.cpp
//  Csucsaramlasmero
//
//  Created by Fehervari Sandor on 2015. 03. 28..
//  Copyright (c) 2015. sandorfehervari. All rights reserved.
//

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "element_extraction.h"
#include "colour_based_extractor.h"
#include "constants.h"
#include "utils.h"

using namespace cv;
using namespace std;

Point calculateRectangleCenter(const Rect& rectangle) {
    Point center(rectangle.x + rectangle.width/2, rectangle.y + rectangle.height /2);
    return center;
}

Rect extractIndicator(Mat& hsvInputImage) {
    Mat extracted, lower_red, upper_red, lines;
    ColourBasedExtractor lowerRed(Scalar(0,140,100), Scalar(3,255,230));
    lowerRed.ExtractColour(hsvInputImage, lower_red);
    ColourBasedExtractor upperRed(Scalar(168,140,100), Scalar(180,255,255));
    upperRed.ExtractColour(hsvInputImage, upper_red);
    bitwise_or(lower_red, upper_red, extracted);
    lower_red.release();
    upper_red.release();
    Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size(5,5));
    morphologyEx(extracted, extracted, MORPH_CLOSE, kernel);

    cv::Mat b = (cv::Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,1);
    erode(extracted, extracted, b, Point(-1,-1), 3);

    return findBiggestBlob(extracted);
}


Rect extractNumberPlate(cv::Mat& hsvInputImage, cv::Mat& dst) {
    Mat extractedColor(hsvInputImage.size(), CV_8U);
    ColourBasedExtractor colourExtractor(YELLOW_RANGE_START, YELLOW_RANGE_END);
    colourExtractor.ExtractColour(hsvInputImage, extractedColor);
    
    Rect plate = findBiggestBlob(extractedColor, true);
    rectangle( dst, plate.tl(), plate.br(), Scalar(255), CV_FILLED, 8, 0 );
    return plate;
}

double calculatePercentage(double value, double baseline) {
    return value / baseline * 100.0f;
}

void mergeOverlappingBoxes(std::vector<cv::Rect> &inputBoxes, cv::Mat &image, std::vector<cv::Rect> &outputBoxes)
{
    cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1); // Mask of original image
    cv::Size scaleFactor(5,0); // To expand rectangles, i.e. increase sensitivity to nearby rectangles. Doesn't have to be (10,10)--can be anything
    for (int i = 0; i < inputBoxes.size(); i++)
    {
        cv::Rect box = inputBoxes.at(i) + scaleFactor;
        cv::rectangle(mask, box, cv::Scalar(255), CV_FILLED); // Draw filled bounding boxes on mask
    }
    std::vector<std::vector<cv::Point> > contours;
    // Find contours in mask
    // If bounding boxes overlap, they will be joined by this function call
    cv::findContours(mask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    for (int j = 0; j < contours.size(); j++)
    {
        outputBoxes.push_back(cv::boundingRect(contours.at(j)));
    }
    mask.release();
}

void transformCoordinatesWithBaseline(const cv::Rect baseline, vector<cv::Rect>& boundingBoxes) {
    for (int i=0; i<boundingBoxes.size(); i++) {
        Rect boundingBox = boundingBoxes[i];
        boundingBox.x += baseline.x;
        boundingBox.y += baseline.y;
        boundingBoxes[i] = boundingBox;
    }
}

void extractNumberFields(cv::Mat& grayInputImage, cv::Rect& numberPlatePlacement, vector<cv::Rect>& boundingBoxes) {
    Mat output;
    cv::Mat roi = grayInputImage(numberPlatePlacement);
    cv::Mat mask = cv::Mat::zeros(roi.size(), CV_8UC1);
    threshold(roi, output, 90, 255, THRESH_BINARY_INV);
    vector<vector <Point> > regions;
    vector<Rect> rectangles;
    findContours(output, regions, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    for (int i=0; i<regions.size(); i++) {
        Rect boundingBox = boundingRect(regions[i]);
        printf("%f\n", calculatePercentage(boundingBox.height, grayInputImage.size().height));
        if (calculatePercentage(boundingBox.height, grayInputImage.size().height) > 10 / AREA_MULTIPLER) {
            rectangles.push_back(boundingBox);
        }
    }
    mergeOverlappingBoxes(rectangles, mask, boundingBoxes);
    output.release();
    mask.release();
}

void parseHoughCircles(Mat& input, Mat& mask) {
    vector<Vec3f> circles;
    Mat coloredInput;
    input.copyTo(coloredInput);
    cvtColor(coloredInput, coloredInput, CV_GRAY2BGR);
    Mat temp;
    distanceTransform(input, temp, CV_DIST_L2, 3);
    normalize(temp, temp, 0, 1., cv::NORM_MINMAX);
    threshold(temp, temp, .5, 1., CV_THRESH_BINARY);
    cv::Mat dist_8u;
    temp.convertTo(dist_8u, CV_8U);
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    cv::Mat markers = cv::Mat::zeros(temp.size(), CV_32SC1);
    size_t ncomp = contours.size();
    for (int i = 0; i < contours.size(); i++)
        cv::drawContours(markers, contours, i, cv::Scalar::all(i+1), -1);
    circle(markers, cv::Point(5,5), 3, CV_RGB(255,255,255), -1);
    watershed(coloredInput, markers);
    
    // Generate random colors
    std::vector<cv::Vec3b> colors;
    for (int i = 0; i < ncomp; i++)
    {
        int b = cv::theRNG().uniform(0, 255);
        int g = cv::theRNG().uniform(0, 255);
        int r = cv::theRNG().uniform(0, 255);
        
        colors.push_back(cv::Vec3b((uchar)b, (uchar)g, (uchar)r));
    }
    
    // Create the result image
    cv::Mat dst = cv::Mat::zeros(markers.size(), CV_8UC3);
    
    // Fill labeled objects with random colors
    for (int i = 0; i < markers.rows; i++)
    {
        for (int j = 0; j < markers.cols; j++)
        {
            int index = markers.at<int>(i,j);
            if (index > 0 && index <= ncomp)
                dst.at<cv::Vec3b>(i,j) = colors[index-1];
            else
                dst.at<cv::Vec3b>(i,j) = cv::Vec3b(0,0,0);
        }
    }
    
}
