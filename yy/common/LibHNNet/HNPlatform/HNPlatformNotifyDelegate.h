#ifndef __HN_PlatformNotifyDelegate_H__
#define __HN_PlatformNotifyDelegate_H__

#include "HNBaseType.h"
#include "HNNetProtocol/HNProtocolExport.h"
#include <string>

namespace HN
{
	class IPlatformMessageDelegate
	{
	public:
		// 网络断线
		virtual void I_P_M_DisConnect(){}

		// 房间列表
		virtual void I_P_M_RoomList(){}

		// 游戏列表
		virtual void I_P_M_GameList(){}

 		// 平台连接
		virtual void I_P_M_Connect(bool result){}

 		// 平台登录
		virtual void I_P_M_Login(bool result, UINT dwErrorCode){}

		// 平台登录玩家人数
		virtual void I_P_M_LoginUsers(UINT uOnLineCount){}

		// 房间人数更新
		virtual void I_P_M_RoomUserCount(UINT roomID, UINT peopleCount, UINT virtualCount){}

		// 游戏人数更新
		virtual void I_P_M_GameUserCount(DL_GP_RoomListPeoCountStruct* userCount){}

		// 平台注册
		virtual void I_P_M_Regist(MSG_GP_S_Register* registerStruct, UINT ErrorCode){}

		// 新闻系统消息
		virtual void I_P_M_NewsSys(MSG_GR_RS_NormalTalk* pData){}

		// 房间密码
		virtual void I_P_M_RoomPassword(MSG_GP_S_C_CheckRoomPasswd* pData){}
	};
}

#endif	//__HN_PlatformNotifyDelegate_H__

