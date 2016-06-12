LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../LibHNNet)
$(call import-add-path,$(LOCAL_PATH)/../../LibHNUI)
$(call import-add-path,$(LOCAL_PATH)/../../LibHNMarket)
$(call import-add-path,$(LOCAL_PATH)/../../LibHNOpen)

LOCAL_MODULE := hn_lobby_static

LOCAL_MODULE_FILENAME := libHNLobby

LOCAL_SRC_FILES := ../HNLobby/FontConfig.cpp \
				   ../HNLobby/HNUpdate.cpp \
                   ../HNLobby/GameDesk.cpp \
                   ../HNLobby/GameExitLayer.cpp \
                   ../HNLobby/GameInitial.cpp \
                   ../HNLobby/GameLists.cpp \
                   ../HNLobby/GameMatch.cpp \
                   ../HNLobby/GameNotice.cpp \
                   ../HNLobby/GamePlatform.cpp \
                   ../HNLobby/GamePrompt.cpp \
				   ../HNLobby/GameWebView.cpp \
                   ../HNLobby/GameRoom.cpp \
				   ../HNLobby/LayerManger.cpp \
				   ../HNLobby/PlatformConfig.cpp \
                   ../HNLobby/Shop/ProductInfo.cpp \
                   ../HNLobby/Shop/ShopManager.cpp \
                   ../HNLobby/PersionalCenter/BindPhone.cpp \
                   ../HNLobby/PersionalCenter/GameUserDataLayer.cpp \
                   ../HNLobby/PersionalCenter/ModifyPassword.cpp \
                   ../HNLobby/GameMenu/GameLandLayer.cpp \
                   ../HNLobby/GameMenu/GameMenu.cpp \
                   ../HNLobby/GameMenu/GameRegisterLayer.cpp \
                   ../HNLobby/ChildLayer/GameBankLayer.cpp \
                   ../HNLobby/ChildLayer/GameExitChangeLayer.cpp \
                   ../HNLobby/ChildLayer/GamePayLayer.cpp \
                   ../HNLobby/ChildLayer/GameRankingList.cpp \
                   ../HNLobby/ChildLayer/GameSetLayer.cpp \
                   ../HNLobby/ChildLayer/GameStoreLayer.cpp \
				   ../HNLobby/ChildLayer/ServiceLayer.cpp \
				   ../HNLobby/ChildLayer/GameSignLayer.cpp \
				   ../HNLobby/ChildLayer/GameOnlineReward.cpp \
				   ../HNLobby/ChildLayer/GameRechargeRebate.cpp \
				   ../HNLobby/ChildLayer/GamePasswordInput.cpp \
				   ../HNLobby/ChildLayer/GameGiftShop.cpp \
				   ../HNLobby/Popularise/SpreadEditLayer.cpp \
				   ../HNLobby/ChildLayer/NoticeList.cpp
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../Classes \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../Classes/Shop \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../Classes/PersionalCenter \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../Classes/GameMenu \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../Classes/Popularise \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../Classes/ChildLayer


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/.. \
                           $(LOCAL_PATH)/../../../../cocos2d \
                           $(LOCAL_PATH)/../../../../cocos2d/cocos \


LOCAL_EXPORT_LDLIBS := -llog \
                        -lz \
                        -landroid
                        
LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_net_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_ui_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_market_static 
LOCAL_WHOLE_STATIC_LIBRARIES += hn_logic_static
LOCAL_WHOLE_STATIC_LIBRARIES += hn_open_static 

include $(BUILD_STATIC_LIBRARY)

$(call import-module,.)
$(call import-module,LibHNMarket)
$(call import-module,LibHNUI)
$(call import-module,LibHNNet)
$(call import-module,LibHNOpen)
