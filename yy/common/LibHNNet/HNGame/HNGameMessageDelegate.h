#ifndef __HN_GameMessageDelegate_H__
#define __HN_GameMessageDelegate_H__

#include "HNBaseType.h"
#include "HNNetProtocol/HNProtocolExport.h"
#include <vector>

namespace HN
{
	
	class IGameMessageDelegate	: public IGameMatchMessageDelegate
								, public IGameChartMessageDelegate
								, public IUserActionMessageDelegate
	{
	public:		
		// 网络断开
		virtual void I_R_M_DisConnect() {}

		// 用户同意
		virtual void I_R_M_UserAgree(MSG_GR_R_UserAgree* agree) = 0;

		// 游戏开始消息
		virtual void I_R_M_GameBegin(BYTE bDeskNO) = 0;
		
		// 结算消息
		virtual void I_R_M_GamePoint(void * object, INT objectSize) = 0;
		
		// 游戏结束消息
		virtual void I_R_M_GameEnd(BYTE bDeskNO) = 0;

		// 游戏信息消息
		virtual void I_R_M_GameInfo(MSG_GM_S_GameInfo* pGameInfo) = 0;

		// 游戏状态消息
		virtual void I_R_M_GameStation(void* object, INT objectSize) = 0;

		// 游戏消息
		virtual bool onGameMessage(NetMessageHead* messageHead, void* object, INT objectSize) = 0;
	};
}

#endif	//__HN_GameMessageDelegate_H__

