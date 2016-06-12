#ifndef __PLATFORMFRAME_SCENE_H__
#define __PLATFORMFRAME_SCENE_H__

#include "HNLobby/ChildLayer/GameStoreLayer.h"
#include "HNLobby/Popularise/SpreadEditLayer.h"
#include "HNNetExport.h"
#include "HNUIExport.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "HNLogicExport.h"
#include <string>
#include <vector>
#include "network/HttpClient.h"

using namespace ui;
using namespace cocostudio;

enum LayerType
{
	ROOMLIST = 0,
	DESKLIST
};

class GameUserDataLayer;
class GameNotice;

class GamePlatform : public HNLayer, public MoneyChangeNotify, public HN::IHNPlatformRoomList,
	public HN::IHNPlatformGameList, public HNSpreadLogic::Delegate, public HNHttpDelegate
{
public:
	HNPlatformRoomList*		_roomListLogic;
	HNPlatformGameList*		_gameListLogic;

	struct LOBBY_BOTTOM
	{
		Button* Button_Friend;
		Button* Button_Task;
		Button* Button_Activity;
		Button* Button_Bank;
		Button* Button_Store;
		Button* Button_Rank;

		Layout*	layout;
		Node*   node;

	} _lobbyBotttom;

	struct LOBBY_TOP
	{
		Text* Label_UserName;
		Text* Label_UserMoney;

		UserHead*	UserHead_Head;
		Button*		Button_Return;
		Button*		Button_service;
		Button*		Button_exit;
		Button*		Button_notice;
		Button*		Button_sign;
		Button*		Button_setting;
		Button*		Button_popularize; //推广图标
		Button*     Button_exchange;//礼品兑换图标
		Layout*	layout;
		Node*   node;
	} _lobbyTop;

	std::map<std::string, cocos2d::String> _Data;

private:
	Label*				_peopleCountLabel;    //
	Sprite*				_Decoration_left;
	Sprite*				_Decoration_right;
	Sprite*				_NoticeBG;
	GameNotice*			_PlatformNotic;
	bool				_isTouch;
	INT					_Time;
	bool				_isCustomer;
	INT					_iValue;
	HNSpreadLogic		_spreadLogic; //推广逻辑类
public:
	static void createPlatform();
	static void returnPlatform(LayerType type);

	void createLayerChoose(LayerType type);

	virtual void onSpreadInfo(bool result, bool bInfo, const std::string& msg) override;

	CREATE_FUNC(GamePlatform);
public:
	GamePlatform();
	virtual ~GamePlatform();

    virtual bool init() override;
	virtual void onExit() override;
	virtual void onEnterTransitionDidFinish() override;

	// 种植种子
	void plantSeed();

	//返回键监听回调函数
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent) override;
	//友盟分享回调
	static void UMengShareCallback(int platform, int stCode,const std::string& errorMsg);

private:
	//查询各种奖励信息
	void checkGiftMessages();
	//卸载奖励监听
	void unSocketCallBack();
	//查询信息回调
	bool checkGiftMessagesEventSelector(HNSocketMessage* socketMessage);
	//领奖提醒倒计时
	void updataGetRewardTimeMessage(float dt);
	//获取随机的提示语
	std::string showHint();

private:
	// 获取房间/游戏列表定时器
	void requestGameRoomTimerCallBack(float dt);
	//顶部按钮回调
	void menuLobbyTopCallBack(Ref* pSender, Widget::TouchEventType type);
	//底部按钮回调
	void menuLobbyBottomCallBack(Ref* pSender, Widget::TouchEventType type);
	// 打开友盟分享组件
	void openUMengShare();
	//截屏保存
	void screenShoot();

public:
	//创建房间列表层
	void createRoomListLayer();
	//创建游戏列表层
	void createGameListLayer();
	//创建桌子列表层
	void createDeskListLayer(ComRoomInfo* roomInfo);

public:
	//顶部和底部变幻动作
	void showTopAndBottomAction(bool isOut, bool canReturn);

public:
	// 监听玩家购买金币消息
	virtual void walletChanged(LLONG money) override;

	// 获取公告请求
	void requestNotice();
	void dealNoticeResp(const std::string& data);

	// 获取功能显示模块
	void requestModule();
	void dealModuleResp(const std::string& data);

	// 获取分享信息
	void requestShare();
	void dealShareResp(const std::string& data);

public:
	////////////////////////////////////////////////////////////////////////////////////////////
	//申请房间列表回调
	virtual void onPlatformRoomListCallback(bool success, const std::string& message) override;
	//更新房间人数
	virtual void onPlatformRoomUserCountCallback(UINT roomID, UINT userCount) override;

	////////////////////////////////////////////////////////////////////////////////////////////
	//申请游戏列表回调
	virtual void onPlatformGameListCallback(bool success, const std::string& message) override;
	//更新游戏人数
	virtual void onPlatformGameUserCountCallback(UINT Id, UINT count) override;

	//断线通知
	virtual void onPlatformDisConnectCallback(const std::string& message) override;
	//新闻广播消息
	virtual void onPlatformNewsCallback(const std::string& message) override;

	virtual void onHttpResponse(const std::string& requestName, bool isSucceed, const std::string &responseData) override;

private:
	//定时赛即将开始
	bool enterTimeMatch();
	//检测报名的定时赛是否开始
	void checkTimeMatchStart(float dt);
	//在游戏列表位置快捷进入房间比赛
	void fastEnterRoom();

	// 播放跳动动画
	void playOrStopJump(cocos2d::Node* pNode, bool bJump);

	// 分享地址
	std::string _shareUrl;

	// 分享内容
	std::string _shareContent;

	// 分享图片
	std::string _shareImage;
};

#endif // __PLATFORMFRAME_SCENE_H__
