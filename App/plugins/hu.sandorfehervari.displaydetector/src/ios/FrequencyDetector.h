//
//  FrequencyDetector.h
//  Analog Display Reader
//
//  Created by Sándor Fehérvári on 2015.02.15..
//
//

#ifndef __Analog_Display_Reader__FrequencyDetector__
#define __Analog_Display_Reader__FrequencyDetector__

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

class FrequencyDetector {
    
private:
    std::vector<int16_t> data;
    float confidency;
    
public:
    FrequencyDetector(std::vector<int16_t>& datap, float confidency) : data(datap), confidency(confidency){}
    int getFrequency();

};

#endif /* defined(__Analog_Display_Reader__FrequencyDetector__) */
