LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../common/LibHNNet)
$(call import-add-path,$(LOCAL_PATH)/../../../common/LibHNUI)
$(call import-add-path,$(LOCAL_PATH)/../../../common/LibHNMarket)
$(call import-add-path,$(LOCAL_PATH)/../../../common/LibHNLobby)
$(call import-add-path,$(LOCAL_PATH)/../../../common/LibHNLogic)

LOCAL_MODULE := FreedomRedMahjong_static

LOCAL_MODULE_FILENAME := libFreedomRedMahjong

LOCAL_SRC_FILES := ../Classes/GameTable/ZYHZ_GameTableLogic.cpp \
                   ../Classes/GameTable/ZYHZ_GameManager.cpp \
                   ../Classes/GameTable/ZYHZ_ResourceLoader.cpp \
				   ../Classes/GameTable/ZYHZ_SouthMahjongCardPool.cpp \
				   ../Classes/GameTable/ZYHZ_CardPool.cpp \
				   ../Classes/GameTable/ZYHZ_GameTableUI.cpp \
				   ../Classes/GameTable/ZYHZ_PoolAction.cpp \
				   ../Classes/GameTable/ZYHZ_HuCard.cpp \
				   ../Classes/GameTable/ZYHZ_Card.cpp \
				   ../Classes/GameTable/ZYHZ_NorthMahjongCardPool.cpp \
				   ../Classes/GameTable/ZYHZ_EastMahjongCardPool.cpp \
				   ../Classes/GameTable/ZYHZ_WestMahjongCardPool.cpp \
				   ../Classes/GameTable/ZYHZ_MahjongCard.cpp \
				   ../Classes/GameTable/ZYHZ_CardPoolUser.cpp \
				   ../Classes/GameTable/ZYHZ_ConcealedKong.cpp \
				   ../Classes/GameTable/ZYHZ_Factory.cpp \
				   ../Classes/GameTable/ZYHZ_MahjongCardPool.cpp \
				   ../Classes/GameTable/ZYHZ_MahjongCardPoolUser.cpp \
				   ../Classes/GameTable/ZYHZ_MeldedKong.cpp \
				   ../Classes/GameTable/ZYHZ_TouchCard.cpp \
				   ../Classes/GameTable/ZYHZ_TouchKong.cpp 
                                  
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