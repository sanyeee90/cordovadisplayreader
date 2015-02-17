//
//  FrequencyDetector.cpp
//  Analog Display Reader
//
//  Created by Sándor Fehérvári on 2015.02.15..
//
//
#include "FrequencyDetector.h"
#include <iostream>
extern "C" {
#include "Yin.h"
}

int FrequencyDetector::getFrequency() {
    int buffer_length = 10;
    Yin yin;
    float pitch=0;
    int16_t* elements = &data[0];
    int size = data.size();
    for (int i=0;i < size; i++){
        std::cout << "orig: " << data[i];
        std::cout << "point: " << elements[i] << std::endl;
    }
    while (pitch < 10 ) {
        Yin_init(&yin, buffer_length, 0.25);
        
		pitch = Yin_getPitch(&yin, elements);
		buffer_length++;
	}
    return buffer_length;
}