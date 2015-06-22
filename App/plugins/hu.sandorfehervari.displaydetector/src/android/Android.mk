LOCAL_PATH := $(call my-dir)
OPENCV_JNI_PATH := sdk/native/jni

COMMON_FILE_PATH := ../common

include $(CLEAR_VARS)

LOCAL_MODULE :=tesseract
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/lib/libtess.so

include $(PREBUILT_SHARED_LIBRARY)

include $(OPENCV_JNI_PATH)/OpenCV.mk

include $(COMMON_FILE_PATH)

LOCAL_MODULE    := DisplayReaderJNI
LOCAL_SRC_FILES := hu_sandorfehervari_analogdisplayreader_DisplayDetectorJNI.cpp \
				   OCREngine.cpp \
				   $(COMMON_FILE_PATH)/algorithm.cpp \
				   $(COMMON_FILE_PATH)/colour_based_extractor.cpp \
				   $(COMMON_FILE_PATH)/element_extraction.cpp \
				   $(COMMON_FILE_PATH)/IndicatorValueDetector.cpp \
				   $(COMMON_FILE_PATH)/utils.cpp
					
LOCAL_LDLIBS +=  -llog -ldl

LOCAL_C_INCLUDES +=  tesseract/include

LOCAL_SHARED_LIBRARIES := tesseract

include $(BUILD_SHARED_LIBRARY)