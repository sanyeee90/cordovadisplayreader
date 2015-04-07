//
//  colour_based_extractor.h
//  Csucsaramlasmero
//
//  Created by Fehervari Sandor on 2015. 03. 28..
//  Copyright (c) 2015. sandorfehervari. All rights reserved.
//

#ifndef __Csucsaramlasmero__colour_based_extractor__
#define __Csucsaramlasmero__colour_based_extractor__

#include "opencv2/core/core.hpp"

using namespace cv;




class ColourBasedExtractor {
    
private:
    Scalar* start_color_range;
    Scalar* end_color_range;
    
public:
    ColourBasedExtractor(Scalar start_color_range, Scalar end_color_range) {
        this->start_color_range = &start_color_range;
        this->end_color_range = &end_color_range;
    }
    
    void ExtractColour(Mat& inputHSVMat, Mat& thresholdedMat);
    
};

#endif /* defined(__Csucsaramlasmero__colour_based_extractor__) */
