#ifndef __HNGameLogicBase_H__
#define __HNGameLogicBase_H__

#include "HNLogic/HNLogicBase.h"
#include "HNNetExport.h"

namespace HN
{
	/*
	 * @brief: redbird game logic interface.
	 */
	class IHNGameLogicBase
	{
	public:
		virtual void onGameDisconnect(){}
	};

	/*
	 * @brief: redbird game logic.
	 */
	class HNGameLogicBase: public HNLogicBase, public IGameMessageDelegate
	{
	public:
		/*
		 * @brief: will start receive message after called.
		 */
		virtual void start() override;
		/*
		 * @brief: will stop receive message after called.
		 */
		virtual void stop() override;
		/*
		 * @brief: send message to room server from client. 
		 */
		virtual void sendGameInfo();
		virtual void sendUserUp();
		virtual void sendUserSit(BYTE lSeatNo);
		virtual void sendForceQuit();
		virtual void sendAgreeGame();
		virtual void sendChatMsg(const std::string& msg);
	
		//防作弊或排队机场重置游戏
		virtual void sendResetGame(bool bReset);
        /*
		 * @brief: translate between view seat no and logic seat no.
		 */
		virtual BYTE viewToLogicSeatNo(BYTE vSeatNO);
		virtual BYTE logicToViewSeatNo(BYTE lSeatNO);

		/*
		 * @brief: get user id from logic seatNo.
		 */
		INT getUserId(BYTE lSeatNo);
		/*
		 * @brief: get my seatNo.
		 */
		BYTE getMySeatNo() const;
		/*
		 * @brief:  get user whether is boy.
		 * @param: 
		 *       lSeatNo is user logic seatNo.
		 * @return: true if it is boy, or false.
		 */
		bool getUserIsBoy(BYTE lSeatNo);
		/*
		 * @brief:  get user's info from userId.
		 * @param:  userID is user's id.
		 * @return: return user's info.
		 */
		UserInfoStruct* getUserByUserID(INT userID);
		/*
		 * @brief:  get user's info from logic seatNo.
		 * @param:  lSeatNo is user's logic seatNo.
		 * @return: return user's info.
		 */
		UserInfoStruct* getUserBySeatNo(BYTE lSeatNo);

		bool isContestGame() { return (RoomLogic()->getRoomRule() & GRR_CONTEST || RoomLogic()->getRoomRule() & GRR_TIMINGCONTEST); }
		bool isQueueGame() { return RoomLogic()->getRoomRule() & GRR_QUEUE_GAME; }

	public:
		HNGameLogicBase(BYTE deskNo, INT maxPlayers, bool autoCreate, IHNGameLogicBase* callback);
		virtual ~HNGameLogicBase();
	private:
		virtual void loadDeskUsers() final;
	protected:		
		virtual void clearDesk();
	protected:
		/*
		 * framework message deal function.
		 */
		virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree);
		virtual void dealGameStartResp(BYTE bDeskNO);
		virtual void dealGameEndResp(BYTE bDeskNO);
		virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);
		virtual void dealQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& users);
		virtual void dealUserUpResp(MSG_GR_R_UserUp * userSit, UserInfoStruct* user);
		virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo);
		virtual void dealGameStationResp(void* object, INT objectSize) = 0;
		virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize) = 0;
		virtual void dealUserCutMessageResp(INT userId, BYTE seatNo);
		virtual void dealGamePointResp(void * object, INT objectSize);
		virtual void dealUserChatMessage(MSG_GR_RS_NormalTalk* normalTalk);

		// 比赛初始化
		virtual void dealGameContestInit(NET_ROOM_CONTEST_CHANGE_RESULT* contestChange);
		//比赛信息广播
		virtual void dealGameContestNotic(MSG_GR_I_ContestInfo* contestInfo);
		// 用户比赛信息
		virtual void dealGameUserContset(MSG_GR_ContestChange* contestChange);
		// 比赛结束
		virtual void dealGameContestOver(MSG_GR_ContestAward* contestAward);
		// 比赛淘汰
		virtual void dealGameContestKick() ;
		// 等待比赛结束
		virtual void dealGameContestWaitOver() ;
		// 比赛奖励
		virtual void dealGameContestAwards(NET_ROOM_CONTEST_AWARD_RESULT* contestAward);
		// 报名数量
		virtual void dealGameContestPeople(NET_ROOM_CONTEST_PEOPLE_RESULT* contestPeople);
		// 个人参赛纪录
		virtual void dealGameContestRecord(NET_ROOM_CONTEST_RECORD_RESULT* ContestRecord);

		/////////////////////////////////////////////////////////////////////////////////////////////
		//防作弊场重置游戏(返回值为true可忽略，为false表示房间满，查找座位失败，需要弹框提示跳转房间层)
		/////////////////////////////////////////////////////////////////////////////////////////////
		virtual void dealGameResetResult(bool success);
		//排队中（需显示排队中字样）
		virtual void dealGameInline(bool bInline);
		/////////////////////////////////////////////////////////////////////////////////////////////
	protected:
		/* 
		 * init member only once.
		 */
		virtual void initParams();
		/*
		 * refresh member before every game start.
		 */
		virtual void refreshParams();
		/*
		 * get next and last user seat no.
		 * @param:  seatNo find seat no.
		 * @return: return seat no if success, or return INVALID_DESKSTATION
		 */
		/*
		 * @brief:  get next user's logic seatNo.
		 */
		virtual BYTE getNextUserSeatNo(BYTE lSeatNo);
		/*
		 * @brief:  get last user's logic seatNo.
		 */
		virtual BYTE getLastUserSeatNo(BYTE lSeatNo);
	private:
		/*
		 * @brief:  platfrom message.
		 */
		virtual void I_R_M_DisConnect() override;

		// 用户同意
		virtual void I_R_M_UserAgree(MSG_GR_R_UserAgree* agree) override;

		// 游戏开始
		virtual void I_R_M_GameBegin(BYTE bDeskNO) override;

		// 游戏结束
		virtual void I_R_M_GameEnd(BYTE bDeskNO) override;

		// 游戏信息
		virtual void I_R_M_GameInfo(MSG_GM_S_GameInfo* pGameInfo) override;

		// 游戏状态
		virtual void I_R_M_GameStation(void* object, INT objectSize) override;
		virtual bool onGameMessage(NetMessageHead* messageHead, void* object, INT objectSize) override;

		// 结算消息
		virtual void I_R_M_GamePoint(void * object, INT objectSize) override;
	
		// 排队用户坐下
		virtual void I_R_M_QueueUserSit(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& users) override;

		// 用户坐下
		virtual void I_R_M_UserSit(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
		
		// 用户站起
		virtual void I_R_M_UserUp(MSG_GR_R_UserUp * userUp, UserInfoStruct* user) override;

		// 用户断线
		virtual void I_R_M_UserCut(INT dwUserID, BYTE bDeskNO, BYTE	bDeskStation) override;
		
		// 比赛初始化
		virtual void I_R_M_ContestInit(NET_ROOM_CONTEST_CHANGE_RESULT* contestChange) override;

		//比赛信息广播
		virtual void I_R_M_ContestNotic(MSG_GR_I_ContestInfo* contestInfo) override;

		// 用户比赛信息
		virtual void I_R_M_UserContest(MSG_GR_ContestChange* contestChange) override;

		// 比赛结束
		virtual void I_R_M_ContestOver(MSG_GR_ContestAward* contestAward) override;

		// 比赛淘汰
		virtual void I_R_M_ContestKick() override;
		
		// 等待比赛结束
		virtual void I_R_M_ContestWaitOver() override;

		// 比赛奖励
		virtual void I_R_M_ContestAwards(std::vector<NET_ROOM_CONTEST_AWARD_RESULT*>* awards) override {}

		// 报名数量
		virtual void I_R_M_ContestPeople(NET_ROOM_CONTEST_PEOPLE_RESULT* contestPeople) override {}

		// 个人参赛纪录
		virtual void I_R_M_ContestRecord(NET_ROOM_CONTEST_RECORD_RESULT* contestRecord) override {}

		// 聊天消息
		virtual void I_R_M_NormalTalk(void* object, INT objectSize) override;
	protected:
		IHNGameLogicBase* _callback;
		HNGameTableUsersData*		_deskUserList;
		BYTE						_deskNo;
		BYTE						_mySeatNo;		
		BYTE						_watchOther;	// 允许旁观
		BYTE						_gameStatus;	// 游戏状态
		bool						_bReset;		// 是否重置游戏
		// + is clockwise, - is opposite.
		int							_seatOffset;	// 座位偏移量
		// before auto action, player's action time.
		BYTE						_waitTime;		// 等待时间
		// max player count in the table.
		INT							_maxPlayers;
		// if the table is auto created
		bool						_autoCreate;
		std::vector<bool>			_existPlayer;
		std::vector<bool>			_playing;
	};
}

#endif