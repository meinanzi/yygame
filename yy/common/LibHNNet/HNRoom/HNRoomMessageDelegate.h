#ifndef __HN_RoomNotifyDelegate_H__
#define __HN_RoomNotifyDelegate_H__

#include "HNBaseType.h"
#include "HNNetProtocol/HNProtocolExport.h"
#include <vector>

namespace HN
{
	class IRoomMessageDelegate	: public IGameMatchMessageDelegate
								, public IGameChartMessageDelegate
								, public IUserActionMessageDelegate
	{
	public:		
		// 网络断开
		virtual void I_R_M_DisConnect(){}

		// 房间连接成功
		virtual void I_R_M_Connect(bool result){}
		
		// 房间登录
		virtual void I_R_M_Login(bool success, UINT handleCode, const std::string& message){}

		// 房间登录完成
		virtual void I_R_M_LoginFinish(){}

		// 用户进入房间
		virtual void I_R_M_UserCome(UserInfoStruct* user){}

		// 用户离开房间
		virtual void I_R_M_UserLeft(UserInfoStruct* user){}

		// 结算消息
		virtual void I_R_M_GamePoint(void * object, INT objectSize){}

		// 封桌
		virtual void I_R_M_LockDesk(BYTE deskNo){}

		// 解封
		virtual void I_R_M_UnLockDesk(BYTE deskNo){}
	};
}

#endif	//__HN_RoomNotifyDelegate_H__

