LOCAL_PATH := $(call my-dir)
OPENCV_JNI_PATH := sdk/native/jni

COMMON_FILE_PATH := ../common
COMMON_FILE_CPP_PATH := ../../common

include $(CLEAR_VARS)

LOCAL_MODULE :=opencvinfo
LOCAL_SRC_FILES :=../sdk/native/libs/$(TARGET_ARCH_ABI)/libopencv_info.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

LOCAL_MODULE := opencvjava
LOCAL_SRC_FILES := ../sdk/native/libs/$(TARGET_ARCH_ABI)/libopencv_java.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE :=leptonica
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/lib/liblept.so


include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE :=tesseract
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/lib/libtess.so


include $(PREBUILT_SHARED_LIBRARY)




include $(OPENCV_JNI_PATH)/OpenCV.mk


LOCAL_MODULE    := DisplayReaderJNI
LOCAL_SRC_FILES := hu_sandorfehervari_analogdisplayreader_DisplayDetectorJNI.cpp \
				   OCREngine.cpp \
				   $(COMMON_FILE_CPP_PATH)/algorithm.cpp \
				   $(COMMON_FILE_CPP_PATH)/colour_based_extractor.cpp \
				   $(COMMON_FILE_CPP_PATH)/element_extraction.cpp \
				   $(COMMON_FILE_CPP_PATH)/IndicatorValueDetector.cpp \
				   $(COMMON_FILE_CPP_PATH)/utils.cpp
					
LOCAL_LDLIBS +=  -llog -ldl

LOCAL_C_INCLUDES +=  tesseract/include
LOCAL_C_INCLUDES +=  $(COMMON_FILE_PATH)

LOCAL_SHARED_LIBRARIES := tesseract
LOCAL_SHARED_LIBRARIES += opencvjava

include $(BUILD_SHARED_LIBRARY)