#include "hu_sandorfehervari_analogdisplayreader_DisplayDetectorJNI.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include "OCREngine.h"

#include "preprocess_image.h"
#include "element_extraction.h"
#include "IndicatorValueDetector.h"
#include "constants.h"
#include "utils.h"

using namespace cv;

static int numbers[] = { 60, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 800};

JNIEXPORT jfloat JNICALL Java_hu_sandorfehervari_analogdisplayreader_DisplayDetectorJNI_processImage
  (JNIEnv * env, jobject jObj, jstring imagePathParam, jstring tesseractData) {
      
      const char *imagePath = env->GetStringUTFChars(imagePathParam, JNI_FALSE);
      const char *tessdataDir = env->GetStringUTFChars(tesseractData, JNI_FALSE);
      cv::Mat imageToProcess = imread(imagePath);
      cv::Mat processed;
      cv::Mat hsvImage, gray;
      cv::Mat filtered_gray;
      float retValue = 0.0f;
      sharpenImage(imageToProcess);
      
      cvtColor(imageToProcess, hsvImage, cv::COLOR_BGR2HSV);
      
      cvtColor(imageToProcess, gray, cv::COLOR_RGB2GRAY);
      
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
      OCREngine ocr(tessdataDir);
      
      for( int i = 0; i < contours.size(); i++ )
      {
          approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
          boundRect[i] = boundingRect( Mat(contours_poly[i]) );
          Mat res = filtered_gray(boundRect[i]);
          int number = ocr.getNumberFromImage(res);
          pointsWithNumbers[i] = std::pair<cv::Point,int>(calculateCenterOfRectangle(boundRect[i]), numbers[i]);
          
      }
      
      retValue = calculateIndicatorPosition(pointsWithNumbers, indicatorPosition);

      return retValue;
  }

