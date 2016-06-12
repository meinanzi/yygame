LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../../../cocos2d/libiconv)
$(call import-add-path,$(LOCAL_PATH)/../../../../common/LibHNNet)
$(call import-add-path,$(LOCAL_PATH)/../../../../common/LibHNUI)
$(call import-add-path,$(LOCAL_PATH)/../../../../common/LibHNMarket)
$(call import-add-path,$(LOCAL_PATH)/../../../../common/LibHNLogic)
$(call import-add-path,$(LOCAL_PATH)/../../../../common/LibHNLobby)
$(call import-add-path,$(LOCAL_PATH)/../../../../common/LibHNOpen)

###### third so begin ########
include $(CLEAR_VARS)
LOCAL_MODULE := DinpayEntryKey
LOCAL_SRC_FILES := prebuilt/armeabi/libDinpayEntryKey.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := entryex
LOCAL_SRC_FILES := prebuilt/armeabi/libentryex.so
include $(PREBUILT_SHARED_LIBRARY)
###### third so end ########

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../../../../cocos2d/external/xxtea/xxtea.cpp \
                   ../../Classes/GameTable/LandlordCardListBoard.cpp \
                   ../../Classes/GameTable/LandlordCardOutList.cpp \
                   ../../Classes/GameTable/LandlordCardTypeAnimation.cpp \
                   ../../Classes/GameTable/LandlordExitPrompt.cpp \
                   ../../Classes/GameTable/LandlordGameAudio.cpp \
                   ../../Classes/GameTable/LandlordGameCardType.cpp \
                   ../../Classes/GameTable/LandlordGameChatDlg.cpp \
                   ../../Classes/GameTable/LandlordGameLogic.cpp \
                   ../../Classes/GameTable/LandlordGamePlayer.cpp \
                   ../../Classes/GameTable/LandlordGameTableLogic.cpp \
				   ../../Classes/GameTable/LandlordGameTableUI.cpp \
                   ../../Classes/GameTable/LandlordGameUserMessageBox.cpp \
                   ../../Classes/GameTable/LandlordPokerCard.cpp \
                   ../../Classes/GameTable/LandlordToolbar.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes/GameMessage \
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes/GameTable

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += libiconv_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_ui_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_net_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_market_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_logic_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_lobby_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_open_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,libiconv)
$(call import-module,LibHNUI)
$(call import-module,LibHNNet)
$(call import-module,LibHNMarket)
$(call import-module,LibHNLogic)
$(call import-module,LibHNLobby)
$(call import-module,LibHNOpen)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
