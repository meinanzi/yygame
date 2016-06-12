#ifndef __GoldenToad_GAMECHAT_LAYER_H__
#define __GoldenToad_GAMECHAT_LAYER_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "HNNetExport.h"
#include "HNUIExport.h"
USING_NS_CC;

using namespace cocostudio;
using namespace ui;

namespace GoldenToad
{

	class GameChatDlgLayer : public HNLayer, public ui::EditBoxDelegate
	{
	public:

		typedef std::function<void (const std::string& text)> SendSpeakeCallBack;
		SendSpeakeCallBack	onSpeakeCallBack;

		typedef std::function<void()> OnCloseCallBack;
		OnCloseCallBack	onCloseCallBack;

		struct CHAT_UI
		{
			ImageView*		ImageView_BG;

			Button*			Button_common;
			Button*			Button_face;
			Button*			Button_send;
			Button*			Button_record;
			HNEditBox*		TextField_text;
			ui::ScrollView* ScrollView_face;
			ui::ListView*	ListView_common;
			ui::ListView*	ListView_chatRecord;

			Widget*			chatUI;

		}_chatUI;

	public:
		GameChatDlgLayer();
		virtual ~GameChatDlgLayer();

	public:
		virtual bool init() override;

		//显示、隐藏聊天界面
		void showOrHideChatDlg(bool bShow);

		//添加聊天记录
		void addChatRecord(std::string msg);

	private:
		void createCommonList(INT idx);

	private:
		//聊天界面按钮回调
		void chatUiButtonCallBack(Ref* pSender, Widget::TouchEventType type);
		//点击表情回调
		void faceTouchEventCallBack(Ref* pSender, Widget::TouchEventType type);
		//常用语列表点击回调
		void commonListEventCallBack(Ref* pSender, ui::ListView::EventType type);

		virtual void editBoxReturn(ui::EditBox* editBox) {};

	public:
		CREATE_FUNC(GameChatDlgLayer);
	};

}

#endif // __GAMECHAT_LAYER_H__