LOCAL_PATH := $(call my-dir)
OPENCV_JNI_PATH := sdk/native/jni

include $(CLEAR_VARS)

LOCAL_MODULE :=tesseract
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/lib/libtess.so

include $(PREBUILT_SHARED_LIBRARY)

include $(OPENCV_JNI_PATH)/OpenCV.mk

LOCAL_MODULE    := DisplayReaderJNI
LOCAL_SRC_FILES := hu_sandorfehervari_analogdisplayreader_DisplayDetectorJNI.cpp \
					colour_based_extractor.cpp \
					element_extraction.cpp \
					IndicatorValueDetector.cpp \
					preprocess_image.cpp \
					utils.cpp \
					OCREngine.cpp
LOCAL_LDLIBS +=  -llog -ldl

LOCAL_C_INCLUDES +=  tesseract/include

LOCAL_SHARED_LIBRARIES := tesseract

include $(BUILD_SHARED_LIBRARY)