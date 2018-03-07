LOCAL_PATH := $(my-dir)

include $(CLEAR_VARS)

LOCAL_CC := $(TARGET_CXX)

LOCAL_MODULE := libPPTClient
LOCAL_SRC_FILES :=./client/PPTClient.cpp

LOCAL_SHARED_LIBRARIES := libutils libbinder_ipc

LOCAL_C_INCLUDES := $(TOPDIR)/base/include \
                    $(TOPDIR)/base/include/common \
                    $(LOCAL_PATH)/com \
                    $(LOCAL_PATH)/client \
                    $(LOCAL_PATH)/include \
                    $(LOCAL_PATH)/service/src/common

include $(BUILD_SHARED_LIBRARY)


#for test
include $(CLEAR_VARS)

LOCAL_LDLIBS := -lhi_msp -lhi_common -lpthread -ljpeg -ldl -lrt
LOCAL_SHARED_LIBRARIES:=libPPTClient libcli libcommon liblog libutils libbinder_ipc

LOCAL_C_INCLUDES := $(TOPDIR)/base/include \
                    $(TOPDIR)/base/include/common \
                    $(TOPDIR)/target/include \
                    $(LOCAL_PATH)/com \
                    $(LOCAL_PATH)/client \
                    $(LOCAL_PATH)/include \
                    $(LOCAL_PATH)/service/src/common
                    

LOCAL_SRC_FILES := ./client/main.cpp


LOCAL_MODULE_TAGS:=eng
LOCAL_MODULE:=PPTClientTest

include $(BUILD_EXECUTABLE)


