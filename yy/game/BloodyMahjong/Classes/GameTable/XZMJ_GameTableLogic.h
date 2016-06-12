#ifndef _XZMJ_TableLogic_H_
#define _XZMJ_TableLogic_H_

#include "XZMJ_MessageHead.h"

namespace XZMJ
{
	class GameTableUICallBack;
	class GameTableUI;
	class GameTableLogic : public HNGameLogicBase
	{
	public:
		GameTableLogic(GameTableUICallBack* tableUICallBack, BYTE deskNo, bool bAutoCreate);
		virtual ~GameTableLogic();

	public:
		virtual void dealGameStartResp(BYTE bDeskNO) override;
		virtual void dealGameEndResp(BYTE bDeskNO) override;

		virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree) override;

		virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
		// 用户站起
		virtual void dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
		// 游戏状态
		virtual void dealGameStationResp(void* object, INT objectSize) override;
		// 游戏消息（游戏的主体消息来自这里）
		virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize) override;
		
	public: 
		//const INT getAgreeCount() { return _agreeCount; }
		//const void addAgreeCount() { ++_agreeCount; }

	public:				// 消息处理
		virtual void setGameStation(const GameStationDataEx& msg);							///<设置状态/// 断线重连消息
		virtual void onUserTuoguan(const Usertouguan& msg);									///玩家托管通知/// 托管消息
		virtual void onUserAgree(const MSG_GR_R_UserAgree& msg);							/// 同意消息    
		virtual void onGameStart(const tagBeginEx& msg);									/// 游戏开始消息
		virtual void onDongNt(const tagDongNtEx& msg);										/// 第0位玩家为庄家消息
		virtual void onSeziSureOpenPoint(const tagTwoSeziDirAndGetPaiEx& msg);				/// 定开门墩骰子
		virtual void onSendAllCard(const tagSendPaiEx& msg);								/// 发牌
		virtual void onGamePlay(const tagBeginOutPaiEx& msg);								/// 开始打牌（出牌）
		virtual void onNotifyOutCard(const tagOutPaiEx& msg);								/// 出牌
		virtual void onNotifyZhuaPai(const tagZhuaPaiEx& msg);								/// 抓牌
		virtual void onNotifyBlock(const tagCPGNotifyEx& msg);								/// 拦牌
		virtual void onNotifyHaveAction(const SC_NotifyHaveAction& msg);                    /// 有动作
		virtual void onNotifyChiPai(const tagChiPai& msg);									/// 吃牌
		virtual void onNotifyPengPai(const tagPengPaiEx& msg);								/// 碰牌
		virtual void onNotifyGangPai(const tagGangPaiEx& msg);								/// 杠牌
		virtual void onNotifyHuPai(const tagHuPaiEx& msg);									/// 胡
		virtual void onNotifySuperSetPai(const SuperStruct& msg);							/// 发牌器发牌
		virtual void onNotifyFinish(const tagCountFenEx& msg);								/// 算分：游戏结束
		virtual void onForceFinish(const tagCountFenEx& msg);								/// 强退结束游戏

	public:			// 
		void safeQuit();
		const GameStationDataEx& getGameStation() { return _msgGameStationDataEx; }

	public:
		sitDir getUserDir(const BYTE& deskStation);
		INT getUserStation(const sitDir& dir);
		int getUserVecIndex(const BYTE& deskStation);
		void reconnected();   
		std::string getHuType(INT type);

	public:       // 消息本地数据
		GameStationDataEx _msgGameStationDataEx;										///<设置状态/// 断线重连消息
		Usertouguan _msgUsertouguan;													///玩家托管通知/// 托管消息
		MSG_GR_R_UserAgree _msgMSG_GR_R_UserAgree;							            /// 同意消息    
		tagBeginEx _msgtagBeginEx;														/// 游戏开始消息
		tagDongNtEx _msgtagDongNtEx;													/// 第0位玩家为庄家消息
		tagTwoSeziDirAndGetPaiEx _msgtagTwoSeziDirAndGetPaiEx;				            /// 定开门墩骰子
		tagSendPaiEx _msgtagSendPaiEx;													/// 发牌
		tagBeginOutPaiEx _msgtagBeginOutPaiEx;											/// 开始打牌（出牌）
		tagOutPaiEx _msgtagOutPaiEx;													/// 出牌
		tagZhuaPaiEx _msgtagZhuaPaiEx;													/// 抓牌
		tagCPGNotifyEx _msgtagCPGNotifyEx;												/// 拦牌
		tagChiPai _msgtagChiPai;														/// 吃牌
		tagPengPaiEx _msgtagPengPaiEx;													/// 碰牌
		tagGangPaiEx _msgtagGangPaiEx;													/// 杠牌
		tagHuPaiEx _msgtagHuPaiEx;														/// 胡
		tagDingQueEx _msgtagDingQueEx;													/// 定缺事件
	    SuperStruct _msgSuperStruct;													/// 发牌器发牌
		tagCountFenEx _msgtagCountFenEx;												/// 算分：游戏结束/// 强退结束游戏				

	private:
		GameTableUICallBack* _tableUICallBack;
		GameTableUI* _callBack;
		
	private:
		UserInfoStruct _userInfo;    // 玩家信息
		bool _hasSetGameStation;                    // 初次进场，重连时用
		//INT _agreeCount;               // 同意玩家数

	public:
		static GameTableLogic* getInstance() { return _instance; }
		static void freeInstance() { delete _instance; _instance = nullptr; }

	private:
		static GameTableLogic* _instance;
		
	};

	#define GTLogic() GameTableLogic::getInstance()

}


#endif // _XZMJ_TableLogic_H_