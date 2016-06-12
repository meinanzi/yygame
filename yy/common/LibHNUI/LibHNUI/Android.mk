LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../LibHNMarket)
$(call import-add-path,$(LOCAL_PATH)/../../LibHNNet)

LOCAL_MODULE := hn_ui_static

LOCAL_MODULE_FILENAME := libHNUI

LOCAL_SRC_FILES := ../UI/Base/HNLayer.cpp \
                   ../UI/Base/HNLayerColor.cpp \
                   ../UI/Base/HNNode.cpp \
                   ../UI/Base/HNScene.cpp \
                   ../UI/Base/HNSprite.cpp \
                   ../UI/Wrapper/HNWrapper.cpp \
                   ../UI/Wrapper/HNSwitchImageViewWrapper.cpp \
                   ../UI/HNDialogBase.cpp \
                   ../UI/LoadingLayer.cpp \
                   ../UI/TextSprite.cpp \
                   ../UI/HNEditBox.cpp \
                   ../UI/HNShake.cpp \
                   ../UI/UserHead.cpp \
                   ../Sound/HNAudioEngine.cpp \
                   ../Tool/Tools.cpp

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
LOCAL_WHOLE_STATIC_LIBRARIES += hn_market_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_net_static

include $(BUILD_STATIC_LIBRARY)
                        
$(call import-module,.)
$(call import-module,audio/android)
$(call import-module,editor-support/spine)
$(call import-module,extensions)
$(call import-module,LibHNMarket)
$(call import-module,LibHNNet)