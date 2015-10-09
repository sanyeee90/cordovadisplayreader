#include "hu_sandorfehervari_analogdisplayreader_DisplayDetectorJNI.h"
#include "algorithm.h"
#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

static int numbers[] = { 60, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 800};
static const int MAX_IMAGE_WH = 1280;

enum orientation {
    ORIENTATION_NORMAL = 1,
    ORIENTATION_90 = 6,
    ORIENTATION_180 = 3,
    ORIENTATION_270 = 8
};

JNIEXPORT jfloat JNICALL Java_hu_sandorfehervari_analogdisplayreader_DisplayDetectorJNI_processImage
  (JNIEnv * env, jobject jObj, jstring imagePathParam, jstring tesseractData, jint orientation) {
      
      const char *imagePath = env->GetStringUTFChars(imagePathParam, JNI_FALSE);
      const char *tessdataDir = env->GetStringUTFChars(tesseractData, JNI_FALSE);
      cv::Mat imageToProcess = imread(imagePath);

      switch(orientation) {
        case ORIENTATION_90:
            cv::transpose(imageToProcess, imageToProcess);
            cv::flip(imageToProcess, imageToProcess, 1);
            break;
        case ORIENTATION_180:
            cv::flip(imageToProcess, imageToProcess, -1);
            break;
        case ORIENTATION_270:
            cv::transpose(imageToProcess, imageToProcess);
            cv::flip(imageToProcess, imageToProcess, 0);
            break;
        default:
            break;
      }
      PFMResultDetector pfm(imageToProcess, tessdataDir);
      
      float retValue = pfm.runAlgorithm();

      return retValue;
  }

