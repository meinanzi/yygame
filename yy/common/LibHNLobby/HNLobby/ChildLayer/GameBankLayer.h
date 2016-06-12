#ifndef GameBankerLayer_h__
#define GameBankerLayer_h__

#include "HNLobby/PlatformDefine.h"
#include "HNLobby/Shop/ShopManager.h"
#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class GameBankLayer : public HNLayer, public ui::EditBoxDelegate
{
private:
	struct PUBLIC_UI
	{
		Node* bankLoader;
		Layout* bankLayout;
	}_publicUi;

	struct PUBLIC_BTN
	{
		Button* bankBtn;
		Button* dataBtn;
		Button* zhuanzhangBtn;
	}_publicBtn;

	struct BANKDATA_UI
	{
		ImageView* bankDataBG;
		Text* curMoney;
		Text* depositMoney;
	}_bankDataUi;

	struct CAOZUO_UI
	{
		Sprite* caozuoBG;
		CheckBox* cunMoney;
		CheckBox* quMoney;
		HNEditBox* CunorQuMoney;
		HNEditBox* CunorQuMima;
		Button* sureBtn;
	}_caozuoUi;

	struct XIUGAI_UI
	{
		ImageView* xiugaiBG;
		HNEditBox* oldPassWord;
		HNEditBox* newPassWord;
		HNEditBox* surePassWord;
		Button* sureBtn;
	}_xiugaiUi;

	struct ZHUANZHANG_UI
	{
		ImageView* zhuanzhangBG;
		CheckBox* name;
		CheckBox* ID;
		HNEditBox* NameorID;
		HNEditBox* giveMoney;
		HNEditBox* mima;
		Button* sureBtn;
	}_zhuanzhangUi;

private:
	bool					_bankOpen;
	LLONG					_bankMoney;			// 钱柜
	LLONG					_walletMoney;		// 钱包的钱
	MoneyChangeNotify*		_delegate;
public:
	static GameBankLayer* createGameBank();

public:
	GameBankLayer();
	virtual ~GameBankLayer();

	virtual bool init() override;

	void openBank(Node* parent, Vec2 vec, int zorder, int tag);

	void closeBank();

	void setChangeDelegate(MoneyChangeNotify* delegate)
	{
		_delegate = delegate;
	}

private:
	void createOperatorUI(Layout* root);

	void createUserDataUI(Layout* root);

	void createForwardUI(Layout* root);

	void createBankInfoUI(Layout* root);

	// 界面消息回调函数
private:
	void closeEvent(Ref* pSender, Widget::TouchEventType type);
	void navigationCheckBoxEvent(Ref* pSender, Widget::TouchEventType  type);

private:
	// 银行操作处理函数
	bool bankerEventSelector(HNSocketMessage* socketMessage);
	// 银行转账处理函数
	bool bankerForwardSelector(HNSocketMessage* socketMessage);

	void openbankResult(TMSG_GP_BankInfo* pBankInfo);//打开银行结果

	void SaveAndOpenBank(void* pDate, int isize);

private:
	void operatorUIEventCheckBoxCallBack(Ref* pSender, CheckBox::EventType type);
	void operatorUIEventCallBack(Ref* pSender, Widget::TouchEventType type);

private:
	void userDataUIEventCallBack(Ref* pSender, Widget::TouchEventType type);

private:
	void forwardUIEventCallBack(Ref* pSender, Widget::TouchEventType type);
	void forwardUICheckBoxEvent(Ref* pSender, CheckBox::EventType type);


	virtual void editBoxReturn(ui::EditBox* editBox) {};
};


#endif // GameBankerLayer_h__
