#ifndef __GameMatch_h__
#define __GameMatch_h__

#include "HNUIExport.h"
#include "HNNetExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace ui;
using namespace cocostudio;

class GameMatch : public HNLayer, IRoomMessageDelegate
{
private:
	struct MATCH_LAYOUT
	{
		Layout*		MatchInfoLayout;
		Text*				Label_ChampionCount;			// 夺冠次数
		Text*				Label_RankingCount;				// 最佳名次
		Text*				Label_MatchCount;					// 比赛次数
		ListView*	ListView_Reward;

	} _matchLayout;

	struct MATCH_OPERATOR_LAYOUT
	{
		Button*		Button_JoinMatch;
		Button*		Button_UnJoinMatch;
		Text*				Label_SignedCount;
		Text*				Label_NeedCount;

	} _operatorLayout;

	int						_btnIndex;									//报名、退赛按钮的切换	
	ImageView*	_Time;											//倒计时数字图片
	bool					_isOver;										//报名人数是否达到
	bool					_isJoin;											//是否报名
	Text*					_beginTimeText;						//定时赛开赛时间
	int						_timeMatchIndex;					//本房间使用的定时赛数据序号
	int						_iValue;											//保存i的值

public:
	static void createMatch();

public:
	GameMatch();
	virtual ~GameMatch();

public:
	virtual bool init() override;

	// 界面接口函数
public:
	// 跳转到房间列表
	void returnRoom(Ref* pSender, Widget::TouchEventType type);

	void joinMatchCallBack(Ref* pSender, Widget::TouchEventType type);

	// 定时器回调函数
public:
	void countDown(float delay);

	void timeMatchCountDown(float delay);

	//查找当前比赛房的开始时间
	std::string stringToTime(std::string beginTime);		

	//退出房间时清除定时赛比赛MARK为1的数据
	void cleanTimeMatchData();

	//退出房间
	void exitMatchRoom();

	//房间过期
	void overdue(float dt);

	//检测报名的定时赛是否开始
	void checkTimeMatchStart(float dt);

	//定时赛等待界面
	bool waitTimeMatch();

public:
	//报名界面初始化
	bool contestInit();

	//报名与退赛消息处理
	bool contestRegistrationResult(HNSocketMessage* socketMessage);

public:
	// 排队用户坐下
	virtual void I_R_M_QueueUserSit(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& users);// override;

	// 比赛结束
	virtual void I_R_M_ContestOver(MSG_GR_ContestAward* contestAward); 

	//比赛信息广播-------->更新报名人数
	virtual void I_R_M_ContestNotic(MSG_GR_I_ContestInfo* contestInfo);

public:
	CREATE_FUNC(GameMatch);
};

#endif // __GameMatch_h__