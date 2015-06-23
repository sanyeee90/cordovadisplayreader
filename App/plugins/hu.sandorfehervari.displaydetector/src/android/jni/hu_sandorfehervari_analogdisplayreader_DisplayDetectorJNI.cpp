#include "hu_sandorfehervari_analogdisplayreader_DisplayDetectorJNI.h"
#include "algorithm.h"
#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

static int numbers[] = { 60, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 800};

JNIEXPORT jfloat JNICALL Java_hu_sandorfehervari_analogdisplayreader_DisplayDetectorJNI_processImage
  (JNIEnv * env, jobject jObj, jstring imagePathParam, jstring tesseractData) {
      
      const char *imagePath = env->GetStringUTFChars(imagePathParam, JNI_FALSE);
      const char *tessdataDir = env->GetStringUTFChars(tesseractData, JNI_FALSE);
      cv::Mat imageToProcess = imread(imagePath);
      
      float retValue = readResultFromPFM(imageToProcess, tessdataDir);

      return retValue;
  }

