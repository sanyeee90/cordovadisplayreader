#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/features2d/features2d.hpp>
#include "opencv2/highgui/highgui.hpp"

#include "element_extraction.h"
#include "OCREngine.h"
#include "utils.h"

#include <tesseract/baseapi.h>

#include <iostream>
#include "line.h"
extern "C" {
#include "Yin.h"
}

using namespace std;


float calculateIndicatorPosition(vector<pair<Point, int>> numberPoints, Point indicatorLocation);

int numbers[] = { 60, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 800};

int main(){

    Mat box = imread("img/pfm20det.png");
 
    Mat blurred;
    Mat hsvImage;
    Mat gray;
    Mat filtered_gray;
    
    GaussianBlur(box, blurred, Size(0,0), 3);
    addWeighted(box, 1.5, blurred, -0.5, 0, box);
    
    
    cvtColor(box, hsvImage, COLOR_BGR2HSV);
    
    
    cvtColor(box, gray, COLOR_RGB2GRAY);
    
    // ---- INDICATOR, NUMBER_PLATE START ---- //

    
    Point indicatorPosition = extractIndicator(hsvImage);
    
    Mat bigestYellowBlob(hsvImage.size(), CV_8U);
    extractNumberPlate(hsvImage, bigestYellowBlob);
    
    // ---- INDICATOR, NUMBER_PLATE END ---- //
    
    // ---- NUMBER FIELDS START ---- //

    gray.copyTo(filtered_gray, bigestYellowBlob);
    
    Mat binary(gray.size(), CV_8U, cvScalar(0));

    extractNumberFields(filtered_gray, binary);

    
    // ---- NUMBER FIELDS END ---- //
    
    // ---- NUMBERS START ---- //
    
    cv::Mat b = (cv::Mat_<uchar>(3,3) << 1,1,0,0,1,0,0,1,1);
    
    threshold(filtered_gray, filtered_gray, 110, 180, THRESH_BINARY);
    bitwise_not(filtered_gray, filtered_gray);
    erode(filtered_gray, filtered_gray, b);
    
    // ---- NUMBERS END ---- //
    
    // BOUNDARY OF NUMBERS //
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    
    findContours(binary, contours, hierarchy, RETR_EXTERNAL,  CHAIN_APPROX_SIMPLE, Point(0, 0) );
    
    // BOUNDARY OF NUMBERS END //

    vector<Rect> boundRect( contours.size() );
    
    /// Approximate contours to polygons + get bounding rects and circles
    vector<vector<Point> > contours_poly( contours.size() );
    
    vector<pair<Point, int>> pointsWithNumbers(contours.size());

    
    OCREngine ocr;
    
    for( int i = 0; i < contours.size(); i++ )
    {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
        

        Mat res = filtered_gray(boundRect[i]);
        
        int number = ocr.getNumberFromImage(res);
        
        pointsWithNumbers[i] = pair<Point,int>(calculateCenterOfRectangle(boundRect[i]), numbers[i]);
        
    }

    calculateIndicatorPosition(pointsWithNumbers, indicatorPosition);
    
    waitKey(0);
    return 0;


}

float calculateIndicatorPosition(vector<pair<Point, int>> numberPoints, Point indicatorLocation) {
    sort(numberPoints.begin(), numberPoints.end(), compareByHeight);
    int selectedIndex = 0;
    for (int i = 0; i < numberPoints.size()-1; i++) {
        if(numberPoints[i].first.y < indicatorLocation.y && numberPoints[i+1].first.y > indicatorLocation.y) {
            selectedIndex=i;
        }
    }
    float startInterval, endInterval;
    startInterval = numberPoints[selectedIndex].first.y;
    endInterval = numberPoints[selectedIndex + 1].first.y;
    
    float normalized = endInterval - startInterval;
    
    float percentage = (indicatorLocation.y-startInterval) / normalized;

    float resultAmount = (numberPoints[selectedIndex+1].second - numberPoints[selectedIndex].second) * percentage + numberPoints[selectedIndex].second;
    cout << "interval: " << numberPoints[selectedIndex+1].second << ", min: " << numberPoints[selectedIndex].second << endl;

    cout << "resultAmount: " << resultAmount;
    
    
    return resultAmount;
}


