#ifndef __GameRoom_SCENE_H__
#define __GameRoom_SCENE_H__

#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "HNLogicExport.h"

USING_NS_CC;

using namespace cocostudio;
using namespace ui;

class GameRoom : public HNLayer, public HN::IHNRoomLogicBase
{
public:
	typedef std::function<void ()> CloseCallBack;
	CloseCallBack				onCloseCallBack;
	CloseCallBack				onRunActionCallBack;

	typedef std::function<void (ComRoomInfo* roomInfo)> EnterDeskCallBack;
	EnterDeskCallBack			onEnterDeskCallBack;

	HNRoomLogicBase*			_roomLogic;
private:
	PageView*					_pageViewRooms;						// 房间列表
	Button*						_currentSelectedRoom;			// 列表按钮

	std::vector<Sprite*>		_pagesPrompt;
	INT							_currentPageIdx;
	bool						_isTouch;
	
	std::vector<UINT>				_vecContestRoom;
	bool											_overdueTime;					//获得的开赛时间是否过期
	bool											_fastEnterMatch;
	INT												_roomID;

public:
	CREATE_FUNC(GameRoom);

public:
	GameRoom();
	virtual ~GameRoom();

public:
    virtual bool init() override;
	
	virtual void onExit() override;


public:
	void closeRoomList();

	// 房间密码输入回调
	void onEnterPasswordClickCallback(const std::string& password);

	// 界面回调接口
public:
	// pageView拖动回调函数
	void pageViewMoveCallBack(Ref* pSender, PageView::EventType type);

	// 列表按钮点击回调函数
	void enterRoomEventCallBack(Ref* pSender, Widget::TouchEventType type);

	// 功能接口
private:
	// 创建房间列表
	void createRoomList();
	// 创建房间
	void createRoomPage(std::vector<ComRoomInfo*> pages);
	// 创建房间元素
	Node* createRoomItem(ComRoomInfo* roomInfo);
	
	// 定时器回调函数
public:
	//// 更新房间人数信息定时器
	//void updateRoomPeopleTimerCallBack(float dt);
	//// 连接房间超时定时器
	//void enterRoomOutTimerTimerCallBack(float dt);
	//更新房间人数
	void updateRoomPeopleCount(UINT roomID, UINT userCount);

	// 基类虚接口
public:
	//登陆房间回调
	virtual void onRoomLoginCallback(bool success, const std::string& message, UINT roomID) override;
	virtual void onRoomSitCallback(bool success, const std::string& message, UINT roomID, BYTE deskNo, BYTE seatNo) override;
	virtual void onRoomQueueSitCallback(bool success, const std::string& message, UINT roomID, BYTE deskNo) override;
	virtual void onRoomDisConnect(const std::string &message) override;

	// 房间密码
	virtual void onPlatformRoomPassEnter(bool success, UINT roomId);

	//比赛接口
public:
	// 比赛房间是否创建判断
	bool JudgeMatchRoom(UINT upPeople, UINT roomRule);
	//报名界面初始化
	bool contestInit(HNSocketMessage* socketMessage);
	//比赛房间都在比赛
	bool contestInitError(HNSocketMessage* socketMessage);
	//定时赛开赛时间
	bool contestBeginTime(HNSocketMessage* socketMessage);
	//清除定时赛无用数据
	void cleanTimeMatchData();
	//进入比赛报名界面
	void fastEnterTimeMatch(UINT roomID);
	//获取定时赛房间id
	UINT getTimeMatchRoomID();

};

#endif // __GameRoom_SCENE_H__
