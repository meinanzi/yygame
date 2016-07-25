#ifndef _ZYHZ_GameTableUICallback_H_
#define _ZYHZ_GameTableUICallback_H_

#include "ZYHZ_MessageHead.h"

/************************************************************************/
/* 类型：回调接口                                                        */
/* 功能：声明牌桌逻辑在收到消息时调用界面的函数接口                        */
/************************************************************************/

namespace ZYHZ
{
	class GameTableUICallBack : public IHNGameLogicBase
	{
	public:
		virtual void dealLeaveDesk() = 0;
		virtual void addUser(const sitDir& dir, const UserInfoStruct& user) = 0;                     // 增加玩家
		virtual void removeUser(const sitDir& dir) = 0;                                                                 // 移除玩家
		virtual void agreeGame(const sitDir& dir) = 0;
		virtual void startGame(const bool& autoBegin = true) = 0;

		virtual void outCard(const sitDir& dir, const INT& number) = 0;  // 出牌

		virtual void catchCard(const sitDir& dir, const INT& number, const bool& head = true) = 0;

		virtual void setPlayGame(bool bs = true) = 0;
		virtual void reconnected(
			std::vector<std::vector<INT>>& vvHandCard, 
			std::vector<std::vector<INT>>& vvOutCard, 
			std::vector<std::vector<std::vector<INT>>>& vvvActionCard,
			std::vector<std::vector<INT>>& vvMenPai,
			std::vector<bool>& vTrusteeshep,
			std::vector<INT>& vQueMen,
			sitDir& ntDir) = 0;   // 断线重连
        
        // 买码
        virtual void showMaimaOption(BYTE timer, tagMaiMa* data) = 0;
        virtual void hideMaimaOption() = 0;
        
        virtual void showStartBtn(bool visible) = 0;
	};
}

#endif //_SXZP_GameTableUICallback_H_