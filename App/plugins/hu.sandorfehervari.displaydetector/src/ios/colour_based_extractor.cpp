//
//  colour_based_extractor.cpp
//  Csucsaramlasmero
//
//  Created by Fehervari Sandor on 2015. 03. 28..
//  Copyright (c) 2015. sandorfehervari. All rights reserved.
//

#include "colour_based_extractor.h"


void ColourBasedExtractor::ExtractColour(cv::Mat &inputHSVMat, cv::Mat &thresholdedMat) {
    inRange(inputHSVMat, *start_color_range, *end_color_range, thresholdedMat);
    
}