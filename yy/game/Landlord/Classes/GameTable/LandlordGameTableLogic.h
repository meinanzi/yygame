#ifndef __Landlord_Game_Table_Logic_H__
#define __Landlord_Game_Table_Logic_H__

#include "HNLogicExport.h"
#include "HNNetExport.h"
#include "LandlordMessageHead.h"
#include "LandlordGameLogic.h"

namespace landlord
{
	
	class GameTableUICallback;
	class GameLogic;

	class GameTableLogic: public HN::HNGameLogicBase
	{
	public:
		GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate);
		virtual ~GameTableLogic();	
		virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize);
	protected:
		/*
		 * @func: game message function.
		 * @info: they will be called when game message come, message dispatch from base class.
		 */
		void dealGameBeginResp(void* object, INT objectSize);
		void dealSendAllCardResp(void* object, INT objectSize);
		void dealSendFinishResp(void* object, INT objectSize);
		void dealCallScoreResp(void* object, INT objectSize);
		void dealCallScoreResultResp(void* object, INT objectSize);
		void dealCallScoreFinishResp(void* object, INT objectSize);
		void dealRobNTResp(void* object, INT objectSize);
		void dealRobNTResultResp(void* object, INT objectSize);
		void dealRobNTFinishResp(void* object, INT objectSize);
		void dealBackCardExResp(void* object, INT objectSize);
		void dealAddDoubleResp(void* object, INT objectSize);
		void dealAddDoubleResultResp(void* object, INT objectSize);
		void dealAddDoubleFinishResp(void* object, INT objectSize);
		void dealShowCardResp(void* object, INT objectSize);
		void dealShowCardResultResp(void* object, INT objectSize);
		void dealShowCardFinishResp(void* object, INT objectSize);
		void dealGamePlayResp(void* object, INT objectSize);
		void dealOutCardResultResp(void* object, INT objectSize);
		void dealOutCardResp(void* object, INT objectSize);
		void dealOneTurnOverResp(void* object, INT objectSize);
		void dealNewTurnResp(void* object, INT objectSize);
		void dealAwardPointResp(void* object, INT objectSize);
		void dealContinueEndResp(void* object, INT objectSize);
		void dealNoCallScoreEndResp(void* object, INT objectSize);
		void dealSafeEndResp(void* object, INT objectSize);
		void dealNotSafeEndResp(void* object, INT objectSize);
		void dealAutoResp(void* object, INT objectSize);
	
	
		//比赛信息广播
		virtual void dealGameContestNotic(MSG_GR_I_ContestInfo* contestInfo) override;
		//用户比赛信息
		virtual void dealGameUserContset(MSG_GR_ContestChange* contestChange) override;
		// 比赛淘汰
		virtual void dealGameContestKick() override;
		// 等待比赛结束
		virtual void dealGameContestWaitOver() override;
		// 比赛结束
		virtual void dealGameContestOver(MSG_GR_ContestAward* contestAward) override;

		//一局结束，清除玩家
		void cleanUserInfo();

	public:
		bool getbContestRoom();		//_bContestRoom数值传递

		/*
		 * @func: framework message function.
		 * @info: they will be called when frame message come from base class.
		 */
		virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree);
		virtual void dealGameStartResp(BYTE bDeskNO);
		virtual void dealGameEndResp(BYTE deskNo);
		virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);
		virtual void dealQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& user);
		virtual void dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);
		virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo);
		virtual void dealGameStationResp(void* object, INT objectSize);

		////////////////////////////////////////////////////////////////////
		////聊天接口
		////////////////////////////////////////////////////////////////////
	public:
		virtual void dealUserChatMessage(MSG_GR_RS_NormalTalk* normalTalk) override;

	public:
		void sendQueue();
		virtual void sendUserUp() override;
		virtual void sendForceQuit() override;
		void sendForceQuitPlatform();
		virtual void clearDesk() override;

	public:
		/*
		 * @func: interface for others. 
		 * @info: they will support table ui to get information, or send some message to server.
		 */
		void sendShowStart();
		void sendOutCard(const std::vector<BYTE> cards);
		void sendRobNT(int value);
		void sendDouble(int value);
		void sendShowCard(int value);
		bool sendAuto(bool bAuto);
		void enterGame();
		void loadUsers();
		bool playing();
		BYTE getMySeatNo();
		bool canOutCard(const std::vector<BYTE>& cards);
		BYTE getCardType(const std::vector<BYTE>& cards);
		void outCard(const std::vector<BYTE> cards);
		void autoOutCheck(BYTE vSeatNo, const std::vector<BYTE>& cards);
		void outCheck(BYTE vSeatNo, const std::vector<BYTE>& cards);
	protected:
		GameTableUICallback*	_uiCallback;
		GameLogic               _gameLogic;
		/*
		 * @func: set seat no offset value.
		 * @info: + is clockwise, - is opposite.
		 */
		void setSeatOffset(BYTE seatNo);
		bool playingGame(int state);
		void playerOut(BYTE seatNo);
		bool isFrontCard(const std::vector<BYTE>& cards);
		void showMultiple();
		void showPlayerHead(BYTE lSeatNo);
	protected:
		/* 
		 * init member only once.
		 */
		virtual void initParams();

		/*
		 * refresh member before every game start.
		 */
		virtual void refreshParams();

	protected:
		bool _playerSitted[PLAY_COUNT];
		bool _userReady[PLAY_COUNT];
		bool _playerAuto[PLAY_COUNT];
		BYTE _beginTime; //time for user to ready.
		BYTE _thinkTime; //time for user out card.
		BYTE _robLordTime; //time for user rob NT.
		BYTE _callScoreTime; //time for user call score.
		BYTE _doubleTime; //time for user double.
		int _doubleLimit;
		int _maxLimit;
		GameTaskStruct _taskInfo;
		GameMutipleStruct _multipleInfo;

		int _userCardCount[PLAY_COUNT];
		BYTE _userCardList[PLAY_COUNT][54];
		int  _userCallScore[PLAY_COUNT];
		int  _lastOutCardCount;
		BYTE _lastOutCardList[54];
		int _baseMultiple;
		int _backCardCount;
		BYTE _backCardList[12];
		BYTE _status;
		BYTE _curActionSeatNo;
		BYTE _lordSeatNo;
		BYTE _outCardSeatNo;
		int _robLord[PLAY_COUNT];
		int _doubleValue[PLAY_COUNT];
		bool _userPass[PLAY_COUNT];
		bool _lastTurnPass[PLAY_COUNT];
		int  _deskCardCount[PLAY_COUNT];
		BYTE _deskCardList[PLAY_COUNT][54];
		int  _lastTurnCardCount[PLAY_COUNT];
		BYTE _lastTurnCardList[PLAY_COUNT][54];
		bool _haveLastTurnData;
		int _baseCardCount;
		BYTE _baseCardList[54];

		//记录是否为比赛场准备
		bool _isReadyQueue;
	public:
		// 是否比赛场
		bool	_bContestRoom;                    

		//是否比赛结束 
		bool	_bContestEnd; 

		//比赛一局结束
		bool	_oneTurnEnd;
	};
}


#endif