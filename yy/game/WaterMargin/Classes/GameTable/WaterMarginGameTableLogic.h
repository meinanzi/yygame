#ifndef __HN_WaterMargin_JLBDTableLogic_H__
#define __HN_WaterMargin_JLBDTableLogic_H__


#include "cocos2d.h"
#include "HNUIExport.h"
#include "HNNetExport.h"
#include "HNLogicExport.h"
using namespace std;

namespace WaterMargin
{

	class GameTableUICallback;

	class GameTableLogic : public HN::HNGameLogicBase
	{
	public:
		GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate);
		~GameTableLogic();
	
	protected:
		GameTableUICallback*	_uiCallback;

	public:
		//框架消息
		virtual void dealGameStationResp(void* object, INT objectSize);
		virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize);
		virtual void dealUserUpResp(MSG_GR_R_UserUp * userSit, UserInfoStruct* user);

		void dealScoreResult(void* object, INT objectSize);
		void dealAddNoteResult(void* object, INT objectSize);
		void dealRollResult(void* object, INT objectSize);
		void dealBibeiHistoryResult(void* object, INT objectSize);
		void dealWinScoreResult(void* object, INT objectSize);
		void dealBiBeiResult(void* object, INT objectSize);
		void dealMaLiResult(void* object, INT objectSize);

		
		void sendUpOrDownMessage(bool value);
		void sendAddNoteMessage();
		void sendGetWinMessage(int value);
		void sendStartRollMessage();
		void sendBibeiBtnMessage();
		void sendDaXiaoMessage(int iBibeiType,int iBibeiMax);
		void sendMaLiMessage();

		//数据转换
		int* DataSwitch(int* str);

		//处理比倍层得分
		void handleSecond();
		//处理再次比倍
		void handleBiBeiAgain();
		//MaLi转一次的结果显示
		void handleMaLiViewGold();
		void MaLiJudge();
		//mali结束转入比倍
		void changeMaLiToBB();
		//mali以得分结束
		void changeMaLiToDF();
		//站起处理
		void SendStandUp();

		
		int test2;
	};
}


#endif // __HN_SlotMachine_JLBDTableLogic_H__