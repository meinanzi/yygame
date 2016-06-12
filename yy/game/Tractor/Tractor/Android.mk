LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../common/LibHNNet)
$(call import-add-path,$(LOCAL_PATH)/../../../common/LibHNUI)
$(call import-add-path,$(LOCAL_PATH)/../../../common/LibHNMarket)
$(call import-add-path,$(LOCAL_PATH)/../../../common/LibHNLobby)
$(call import-add-path,$(LOCAL_PATH)/../../../common/LibHNLogic)


LOCAL_MODULE := Tractor_static

LOCAL_MODULE_FILENAME := libTractor

LOCAL_SRC_FILES := ../Classes/GameTable/TRACTORDrawEllipse.cpp \
                   ../Classes/GameTable/TRACTORGameCardType.cpp \
				   ../Classes/GameTable/TRACTORGameChipUI.cpp \
				   ../Classes/GameTable/TRACTORGameDashboard.cpp \
				   ../Classes/GameTable/TRACTORGameDealerUI.cpp \
				   ../Classes/GameTable/TRACTORGameLogic.cpp \
                   ../Classes/GameTable/TRACTORGameNotice.cpp \
                   ../Classes/GameTable/TRACTORGamePlayer.cpp \
				   ../Classes/GameTable/TRACTORGameResult.cpp \
                   ../Classes/GameTable/TRACTORGameTableLogic.cpp \
                   ../Classes/GameTable/TRACTORGameTableUI.cpp \
                   ../Classes/GameTable/TRACTORGameUserMessageBox.cpp \
                   ../Classes/GameTable/TRACTORPokerCard.cpp \
                                  
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../Classes/GameMessage \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../Classes/GameTable \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../ \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../ \

LOCAL_EXPORT_LDLIBS := -llog \
                       -lz \
                       -landroid     
                   
LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

LOCAL_WHOLE_STATIC_LIBRARIES += hn_ui_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_net_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_market_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_lobby_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_logic_static

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