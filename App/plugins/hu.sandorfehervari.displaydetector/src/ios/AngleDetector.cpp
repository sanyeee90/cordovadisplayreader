//
//  AngleDetector.cpp
//  Analog Display Reader
//
//  Created by Sándor Fehérvári on 2015.02.12..
//
//

#include "AngleDetector.h"
#include "line.h"

using namespace cv;

void AngleDetector::psdt() {
    Mat planes[] = { Mat_<float>(imageData), Mat::zeros(imageData.size(), CV_32F) };
    
	Mat complexI;
	merge(planes, 2, complexI);         // Add to the expanded another plane with zeros
	dft(complexI, complexI, DFT_COMPLEX_OUTPUT + DFT_SCALE);            // this way the result may fit in the source matrix
    
	Mat result;
	//result = complexI;
	mulSpectrums(complexI, complexI, result, DFT_SCALE, true);
    
	Mat normal;
	dft(result, normal, DFT_INVERSE + DFT_REAL_OUTPUT);
	normalize(normal, normal, 0, 1, CV_MINMAX);
    
    
	Mat resultplanes[2];
	split(result, resultplanes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    
	Mat magI;
	magnitude(resultplanes[0], resultplanes[1], magI); // planes[0] = magnitude
    
	
	magI += Scalar::all(1);                    // switch to logarithmic scale
	log(magI, magI);
	
	// crop the spectrum, if it has an odd number of rows or columns
	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
    
	// rearrange the quadrants of Fourier image  so that the origin is at the image center
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;
    
	Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right
    
	Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
    
	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);
    
	normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
	magI.copyTo(outputData);
}

float AngleDetector::getAngleValue() {
    psdt();
    Mat input, dst, cdst;
	Mat temp;
	double Max, Min;
    
    imageData.copyTo(input);
    
	minMaxLoc(input, &Min, &Max);
    outputData.copyTo(input);
	input *= 255.0;
    
	input.convertTo(temp, CV_8U);
	line(temp, Point(temp.cols / 2, 0), Point(temp.cols / 2, temp.rows), Scalar(0, 0, 0), 3);
	circle(temp, Point(temp.cols/2, temp.rows/2), 5, Scalar(0, 0, 0),-1);
	Point minLoc, maxLoc;
    resize(temp, temp, Size(temp.rows*1.5, temp.cols*1.5));
	minMaxLoc(temp, &Min, &Max, &minLoc, &maxLoc);
	dst = temp;
    
	cvtColor(dst, cdst, CV_GRAY2BGR);
	Point center(temp.cols / 2, temp.rows / 2);
	line(cdst, maxLoc, center, Scalar(0, 255, 0), 1);
    
	float dx = abs(maxLoc.x - center.x);
	float dy = abs(maxLoc.y - center.y);
    
	detected_angle = atan2(dy, dx);
	detected_angle *= (180 / CV_PI);

    cdst.copyTo(outputData);
	return detected_angle;
}

void AngleDetector::getImageDataAtAngle(std::vector<int16_t>& inputVector) {
    
    Point P1(imageData.cols / 2, imageData.rows / 2);
    Line generatedLine(detected_angle, P1);
    
    vector<int16_t> res = generatedLine.getImageData(imageData);
    generatedLine.applyToImage(imageData, Scalar(0,255,255));
    inputVector.reserve( res.size() );
    copy(res.begin(), res.end(), std::back_inserter(inputVector));

}