#ifndef __HN__GoldenToad_GameTableLogic_H__
#define __HN__GoldenToad_GameTableLogic_H__


#include "cocos2d.h"
#include "HNLogicExport.h"
#include "HNNetExport.h"
#include "GoldenToadMessageHead.h"

using namespace std;
using namespace HN;

namespace GoldenToad
{
	class GameTableUICallback;

	class GameTableLogic : public HNGameLogicBase
	{
		GameTableUICallback*			_tableUICallBack;

	public:
		BYTE							_MYSEATNO;
		bool							_playerSitted[PLAY_COUNT];			//玩家是否已经入座
		std::vector<OnLineUserInfo>		_users;								//在线玩家列表

	public:
		GameTableLogic(GameTableUICallback* tableUICallBack, BYTE deskNo, bool bAutoCreate);
		virtual ~GameTableLogic();

	public:
		void enterGame();
		void loadUsers();
		LLONG getUserMoney(BYTE seatNo);
		LLONG getUserCannonNum(BYTE seatNo);
		void setGameConfig();
		BYTE getUserViewSeatNo(BYTE seatNo);

	public:
		//玩家开火
		void sendOpenFire(CMD_C_UserFire* pFire, INT size);
		//抓到鱼
		void sendHitFish(BYTE seatNo, INT fishId, INT bulletId, BulletKind bulletKind, INT bulletMuriple);
		//报告服务器抓到大鱼时当前屏幕中的其他鱼
		void sendSweepFish(CMD_C_CatchSweepFish* pRet, INT size);

		//查询奖池操作
		void sendStockOperate(BYTE operate_code);

	private:
		// 游戏消息（游戏的主体消息来自这里）
		virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize) override;

		/*
		 * @func: framework message function.
		 * @info: they will be called when frame message come from base class.
		 */
		virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree) {};
		virtual void dealGameStartResp(BYTE bDeskNO) override;
		virtual void dealGameEndResp(BYTE deskNo) override;
		virtual void dealGamePointResp(void* object, INT objectSize) override;
		virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
		virtual void dealQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& user) override;
		virtual void dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
		virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo) override;
		virtual void dealGameStationResp(void* object, INT objectSize) override;


	private:
		virtual void initParams() override;
		/*
		 * refresh member before every game start.
		 */
		virtual void refreshParams() override;


		////////////////////////////////////////////////////////////////////
		////聊天接口
		////////////////////////////////////////////////////////////////////
	public:
		virtual void dealUserChatMessage(MSG_GR_RS_NormalTalk* normalTalk) override;
	};

}


#endif // __HN_GoldenToad_GameTableLogic_H__