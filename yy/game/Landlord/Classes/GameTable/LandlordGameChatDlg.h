#ifndef __landlord_GAMECHAT_LAYER_H__
#define __landlord_GAMECHAT_LAYER_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "HNNetExport.h"
#include "HNUIExport.h"
USING_NS_CC;

using namespace cocostudio;
using namespace ui;

namespace landlord
{

	class GameChatDlgLayer : public HNLayer, public ui::EditBoxDelegate
	{
	public:

		typedef std::function<void (const std::string& text)> SendSpeakeCallBack;
		typedef std::function<void (ui::ScrollView* chatScroll)> SpeakeRecordCallBack;
		SendSpeakeCallBack	 onSpeakeCallBack;
		SpeakeRecordCallBack onRecordCallBack;
		struct CHAT_UI
		{
			ImageView*		ImageView_BG;

			Button*			Button_common;
			Button*			Button_face;
			Button*			Button_chat;
			Button*			Button_send;
			HNEditBox*		TextField_text;
			ui::ScrollView* ScrollView_face;
			ui::ScrollView* ScrollView_chat;
			ui::ListView*	ListView_common;
			ui::ListView*	ListView_chatRecord;

			Widget*			chatUI;

		}_chatUI;

	public:
		GameChatDlgLayer();
		virtual ~GameChatDlgLayer();

	public:
		virtual bool init() override;

		void closeChatDlg();

	private:
		void createCommonList(INT idx);
	private:
		//聊天界面按钮回调
		void chatUiButtonCallBack(Ref* pSender, Widget::TouchEventType type);
		//点击表情回调
		void faceTouchEventCallBack(Ref* pSender, Widget::TouchEventType type);
		//常用语列表点击回调
		void commonListEventCallBack(Ref* pSender, ui::ListView::EventType type);
	public:
		CREATE_FUNC(GameChatDlgLayer);
		//添加聊天记录
		void addChatRecord(std::vector<std::string > msg);
        
        virtual void editBoxReturn(ui::EditBox* editBox) {};
	};

}

#endif // __GAMECHAT_LAYER_H__