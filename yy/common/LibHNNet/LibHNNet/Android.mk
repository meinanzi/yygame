LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../LibHNUI)

LOCAL_MODULE := hn_net_static

LOCAL_MODULE_FILENAME := libHNNet

LOCAL_SRC_FILES := ../HNExternal/MD5/MD5.cpp \
                   ../HNPlatform/HNPlatformBase.cpp \
                   ../HNPlatform/HNPlatformLogic.cpp \
                   ../HNPlatform/HNGameCreator.cpp \
                   ../HNRoom/HNRoomLogic.cpp \
                   ../HNSocket/HNSocket.cpp \
                   ../HNSocket/HNSocketLogic.cpp \
                   ../HNSocket/HNSocketProtocolData.cpp \
                   ../HNSocket/HNSocketThread.cpp \
                   ../HNSocket/HNSocketMessage.cpp \
                   ../HNGame/HNGameTableUsersData.cpp \
                   ../HNGame/HNGameUIBase.cpp \
                   ../HNData/HNUserInfoModule.cpp \
                   ../HNData/HNRoomInfoModule.cpp \
                   ../HNData/HNGamesInfoModule.cpp \
                   ../HNCommon/HNLog.cpp \
                   ../HNCommon/HNThread.cpp \
                   ../HNCommon/HNMemoryPool.cpp \
                   ../HNCommon/HNConverCode.cpp \
				   ../HNNetProtocol/HNBaseCommand.cpp \
				   ../HNHttp/HNHttp.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/.. \
                           $(LOCAL_PATH)/../../../../cocos2d \
                           $(LOCAL_PATH)/../../../../cocos2d/cocos \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \

LOCAL_EXPORT_LDLIBS := -llog \
                       -lz \
                       -landroid
                        
LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES := hn_ui_static

include $(BUILD_STATIC_LIBRARY)
                        
$(call import-module,.)
$(call import-module,LibHNUI)
