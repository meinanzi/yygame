LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../common/LibHNNet)
$(call import-add-path,$(LOCAL_PATH)/../../../common/LibHNUI)
$(call import-add-path,$(LOCAL_PATH)/../../../common/LibHNMarket)
$(call import-add-path,$(LOCAL_PATH)/../../../common/LibHNLobby)
$(call import-add-path,$(LOCAL_PATH)/../../../common/LibHNLogic)


LOCAL_MODULE := Landlord_static

LOCAL_MODULE_FILENAME := libLandlord

LOCAL_SRC_FILES := ../Classes/GameTable/LandlordCardListBoard.cpp \
                   ../Classes/GameTable/LandlordCardOutList.cpp \
                   ../Classes/GameTable/LandlordCardTypeAnimation.cpp \
                   ../Classes/GameTable/LandlordGameAudio.cpp \
                   ../Classes/GameTable/LandlordGameCardType.cpp \
                   ../Classes/GameTable/LandlordGameLogic.cpp \
                   ../Classes/GameTable/LandlordGamePlayer.cpp \
				   ../Classes/GameTable/LandlordGameChatDlg.cpp \
                   ../Classes/GameTable/LandlordGameTableLogic.cpp \
                   ../Classes/GameTable/LandlordGameTableUI.cpp \
                   ../Classes/GameTable/LandlordGameUserMessageBox.cpp \
                   ../Classes/GameTable/LandlordPokerCard.cpp \
                   ../Classes/GameTable/LandlordToolbar.cpp \
				   ../Classes/GameTable/LandlordExitPrompt.cpp   
				   
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../Classes/GameMessage \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../Classes/GameTable \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../ \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../ \

LOCAL_EXPORT_LDLIBS := -llog \
                       -lz \
                       -landroid     
                   
LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_ui_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_net_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_market_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_lobby_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_logic_static

LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_STATIC_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)
$(call import-module,editor-support/cocostudio)
$(call import-module,extensions)

$(call import-module,LibHNUI)
$(call import-module,LibHNNet)
$(call import-module,LibHNMarket)
$(call import-module,LibHNLogic)
$(call import-module,LibHNLobby)