//
//  IndicatorValueDetector.cpp
//  Analog Display Reader
//
//  Created by Fehervari Sandor on 2015. 03. 29..
//
//

#include "IndicatorValueDetector.h"
#include "utils.h"
#include "EmptyImageException.h"
#include "constants.h"
using namespace std;
using namespace cv;



float calculateIndicatorPosition(vector<pair<Point, int> >& numberPoints, Point& indicatorLocation) {
    if(numberPoints.empty()) {
        throw AlgorithmException(ERROR_FAILED_TO_DETECT_NUMBER_FIELDS_DIGIT);
    }
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
    return resultAmount;
}