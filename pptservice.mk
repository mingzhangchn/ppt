LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_CC := $(TARGET_CXX)

ifeq ($(CFG_BOARD_HI3716DVlDMA_HI3716DV100), y)
LOCAL_CFLAGS += -DHI_BOARD_HI3716DVlDMA_HI3716DV100
endif
ifeq ($(CFG_BOARD_HI3796MDMO1A_HI3796MV100), y)
LOCAL_CFLAGS += -DHI_BOARD_HI3796MDMO1A_HI3796MV100
endif


LOCAL_SRC_FILES := service/main.cpp
LOCAL_SRC_FILES += service/PPTChannelManager.cpp
LOCAL_SRC_FILES += service/PPTTsFilterMgr.cpp
LOCAL_SRC_FILES += service/PPTTsSliceMgr.cpp
LOCAL_SRC_FILES += service/PPTTunerManager.cpp
LOCAL_SRC_FILES += service/PPTService.cpp
LOCAL_SRC_FILES += service/ppt_create_psiTable.c
LOCAL_SRC_FILES += service/ppt_web_service.c
LOCAL_SRC_FILES += service/PPTNctFilter.cpp
LOCAL_SRC_FILES += service/PPTNctSectionManager.cpp
LOCAL_SRC_FILES += service/PPTNctParser.cpp
LOCAL_SRC_FILES += service/PPTDDTFilter.cpp
LOCAL_SRC_FILES += service/PPTDDTSectionManager.cpp
LOCAL_SRC_FILES += service/PPTDDTParser.cpp
LOCAL_MODULE := service_ppt

LOCAL_LDLIBS := -lhi_msp -lhi_common -ljpeg -ldl -lupnp -lixml -lthreadutil -lpthread -lxml2
LOCAL_SHARED_LIBRARIES :=libDTVClient libcli libcommon liblog libutils libbinder_ipc libhal  liblog libutils 

LOCAL_C_INCLUDES := $(TOPDIR)/base/include \
                    $(TOPDIR)/base/include/common \
                    $(TOPDIR)/target/include \
                    $(TOPDIR)/framework/appmanager/include \
                    $(TOPDIR)/framework/resmanager/include \
                    $(TOPDIR)/framework/systemservice/systemservice_ipc/server/ \
                    $(TOPDIR)/framework/systemservice/systemservice_ipc/client/ \
                    $(TOPDIR)/framework/systemservice/include/ \
                    $(TOPDIR)/component/db/ \
                    $(LOCAL_PATH)/service/ \
                    $(LOCAL_PATH)/include/ 

include $(BUILD_EXECUTABLE)
