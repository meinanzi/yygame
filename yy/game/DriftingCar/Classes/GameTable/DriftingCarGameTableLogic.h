#ifndef __DriftingCar_Game_Table_Logic_H__
#define __DriftingCar_Game_Table_Logic_H__

#include "HNNetExport.h"
#include "HNLogicExport.h"
#include "DriftingCarMessageHead.h"
#include <array>
#include <stdexcept>
namespace DriftingCar
{
	template <typename T, typename TArray>
	void CopyArray(const T* pSou, size_t size, TArray &array)
	{
		if(size > array.size())
			throw std::out_of_range("out of range") ;
		std::copy(pSou,pSou+size, array.begin());
	}

	class GameTableUICallback;

	class GameTableLogic: public HN::HNGameLogicBase
	{
	public:
		GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate);
		virtual ~GameTableLogic();	
	public:
		//框架消息
		virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)override;//游戏消息
		virtual void dealGameStationResp(void* object, INT objectSize)override;							//处理游戏场景消息
		virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree)override;								//游戏同意消息
		virtual void dealGameStartResp(BYTE bDeskNO)override;											//游戏开始
		virtual void dealGameEndResp(BYTE bDeskNO)override;												//游戏结束	
		virtual void dealGamePointResp(void* object, INT objectSize)override;							//结算消息
		virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)override;			//玩家坐下
		virtual void dealUserUpResp(MSG_GR_R_UserUp * userSit, UserInfoStruct* user)override;			//玩家起立
		virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo)override;							//游戏信息
		virtual void dealUserCutMessageResp(INT userId, BYTE seatNo)override;							//断线用户信息

		//游戏消息
		void dealPrepareEndResp(void* object, INT objectSize);
		void dealSuperUserResp(void* object, INT objectSize);	// 超端用户消息
		void dealSuperSendResp(void* object, INT objectSize);	// 超端用户向客户端发送结果
		void dealUpdateBetResp(void* object, INT objectSize);	// 更新下注
		void dealGameFreeResp(void* object, INT objectSize);	// 空闲消息
		void dealBeginBetResp(void* object, INT objectSize);	// 开始下注
		void dealGameSendResp(void* object, INT objectSize);	// 开始开牌 跑马灯开始
		void dealApplyResultResp(void* object, INT objectSize);	// 申请上庄结果
		void dealLeaveResultResp(void* object, INT objectSize);	// 申请下庄结果
		void dealBetResultResp(void* object, INT objectSize);	// 玩家下注结果消息
		void dealGameOpenResp(void* object, INT objectSize);	// 开奖消息 结算消息
		void dealWaitNT(void* object, INT objectSize);			// 等待庄家

	public:
		/*
		 * 
		 */
		void requestApplyNT();									//请求上庄
		void requestApplyLeaveNT();								//请求下庄
		void requestUserBet(BYTE area, BYTE type);				//请求下注
		void requestLeaveTable();								//请求离开
		void ShowGameEndResult();								//显示结算的信息
	protected:

		virtual void initParams();//init member only once.

		virtual void refreshParams();
		
		void clearDesk();							//清除牌桌内容

		void updateNTInfo(BYTE lSeatNo, int NTCount, LLONG grade);

		void updateUserInfo(BYTE lSeatNo, LLONG grade);

		void addNT(BYTE seatNo);						//更新上庄列表（上庄列表不包含庄家）

		void addNT(const std::array<BYTE,PLAY_COUNT>&);

		void removeNT(const std::array<BYTE,PLAY_COUNT>&);

		void reloadRecord(const std::array<BYTE, HISTORY_COUNT>& records, int size, BYTE byResult);//更新历史开奖记录

		void setBetEnable();							//设置下注按钮可用性

		void setApplyButtonEnabled(BYTE NTseatNo);		//设置上庄申请按钮可用性

		void ShowWaitNTListCount(BYTE byseatNo);		//显示当前玩家在等待列表中的位置

		void GetHistroyResult(int indexLogos);			//得到下注结果

	private:
		GameTableUICallback*	_uiCallback;
	private:
		enum
		{
			Result_NON,		//没有下注
			Result_Win,		//压中了
			Result_Lose,	//没有压中
		};
		std::array<BYTE,PLAY_COUNT> _NTWaitList;	//上庄申请列表	
		std::array<BYTE,HISTORY_COUNT> _historyArray;//开奖历史
		std::array<LLONG, BET_ARES> _AreaAllBetArray;//各区域总下注数量
		std::array<LLONG, BET_ARES> _UserBetSumArray;//玩家在各区域下注数量(操作玩家)
		std::array<BYTE, SEND_COUNT> _CardStyleArray;//牌型
		std::array<LLONG, BET_ARES> _PreUserBetArray;//玩家下注区域值
		std::array<INT,TIME_COUNT>	_RunTimeArray;	 //跑马灯时间
		std::array<bool, BET_ARES>  _arrayUserBetArea;//玩家下注区域

		LLONG _lReuslt[4];							 //结算信息

		BYTE _NTSeatNo;								 //庄家座位号
		BYTE _NTCount;								 //坐庄次数
		LLONG _NTGrade;								 //坐庄成绩							

		LLONG _SumAllBet;							 //总的下注数量
		
		LLONG _UserGrade;							 //玩家成绩

		BYTE _GameState;							 //游戏状态
		BYTE _FreeTime;								 //空闲时间
		
		BYTE _WinIndex;								 //开奖位置 和下注位置不同

		BYTE _BetTime;								 //下注时间
		BYTE _SendTime;								 //开奖时间
		BYTE _RemaindTime;							 //剩余时间

		BYTE _MaxNTPeople;							 //最大上庄人数

		BYTE _byHistory;							 //0表示没有下注， 1表示开奖了下注区域，2表示没有开奖下注区域
			
	};
}


#endif