#ifndef __GAMEONLINEREWARD_LAYER_H__
#define __GAMEONLINEREWARD_LAYER_H__

#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio;
using namespace ui;

class GameOnlineReward : public HNLayer
{
	struct OnlineRewardUi
	{
		ImageView*		ImageView_OnlineBG;
		ImageView*		ImageView_Light;
		Layout*			Layout_Wait;
		Layout*			Layout_Get;
		Text*			Text_Time;
		Text*			Text_Money;

		Button*			Button_Get;

	}_OnlineRewardUi;

	INT				_Time;

public:
	typedef std::function<void (LLONG money)> UpdataUserMoney;
	UpdataUserMoney onUpdataUserMoney;

	typedef std::function<void ()> CloseCallBack;
	CloseCallBack	onCloseCallBack;

public:
	GameOnlineReward();
	virtual ~GameOnlineReward();

public:
	virtual bool init() override;

	void closeOnlineReward();

	//按钮回调
	void buttonEventCallBack(Ref* pSender, Widget::TouchEventType type);

private:
	//查询奖励信息
	void checkRewardData();
	//申请查询奖励信息回调
	bool checkRewardDataEventSelector(HNSocketMessage* socketMessage);

	//领奖倒计时
	void updataGetRewardTimeMessage(float dt);

private:
	//申请领取签到奖励回调
	bool getOnlineRewardEventSelector(HNSocketMessage* socketMessage);

public:
	CREATE_FUNC(GameOnlineReward);
};

#endif // __ONLINEREWARD_LAYER_H__
