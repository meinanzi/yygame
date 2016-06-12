LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../LibHNNet)
$(call import-add-path,$(LOCAL_PATH)/../../LibHNMarket)

LOCAL_MODULE := hn_logic_static

LOCAL_MODULE_FILENAME := libHNLogic

LOCAL_SRC_FILES := ../HNLogic/HNLogicBase.cpp \
                   ../HNPlatformLogic/HNPlatformGameList.cpp \
				   ../HNPlatformLogic/HNPlatformLogicBase.cpp \
				   ../HNPlatformLogic/HNPlatformLogin.cpp \
				   ../HNPlatformLogic/HNPlatformRegist.cpp \
				   ../HNPlatformLogic/HNPlatformRoomList.cpp \
				   ../HNPlatformLogic/HNPlatformSpreadLogic.cpp \
				   ../HNPlatformLogic/HNPlatformSpreadManager.cpp \
                   ../HNRoomLogic/HNGameLogicBase.cpp \
				   ../HNRoomLogic/HNRoomDeskLogic.cpp \
				   ../HNRoomLogic/HNRoomLogicBase.cpp \

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/.. \
                           $(LOCAL_PATH)/../../../../cocos2d \
                           $(LOCAL_PATH)/../../../../cocos2d/cocos \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \

LOCAL_EXPORT_LDLIBS := -llog \
                       -lz \
                       -landroid
                        
LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_net_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_market_static

include $(BUILD_STATIC_LIBRARY)
                        
$(call import-module,.)
$(call import-module,audio/android)
$(call import-module,editor-support/spine)
$(call import-module,extensions)
$(call import-module,LibHNNet)
$(call import-module,LibHNMarket)
