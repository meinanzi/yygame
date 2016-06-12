#ifndef _XZMJ_GAMETABLE_H
#define _XZMJ_GAMETABLE_H

/*****************************************************/
// hn.h
#include "XZMJ_MessageHead.h"
#include "XZMJ_GameTableUICallback.h"
#include "XZMJ_GameTableLogic.h"



/*****************************************************/

namespace XZMJ
{
	// namespace
	USING_NS_CC;
	using namespace HN;
	using namespace ui;
	using namespace std;
	class GameManager;
	class GameTableUI : public HNGameUIBase, GameTableUICallBack
	{
	public: // create
		GameTableUI();
		virtual ~GameTableUI();
		static GameTableUI* create(INT deskNo, bool autoCreate);
		bool init(INT deskNo, bool autoCreate);

	public:  // 接口
		// 游戏状态
		virtual void dealLeaveDesk() override;    // 离开桌子
		virtual void addUser(const sitDir& dir, const UserInfoStruct& user) override;     // 进入桌子
		virtual void removeUser(const sitDir& dir);    // 离开桌子
		virtual void agreeGame(const sitDir& dir) override;
		virtual void startGame(const bool& autoBegin) override;

		virtual void touziAction(const sitDir& startDir, const Vec2& startPos, const int& num1, const int& num2, const Vec2& endPos1, const Vec2& endPos2);   // 骰子动画
		virtual void startSendBottomCard(std::vector<std::vector<INT>> vvSouthCard) override;  // 发底牌
		Vector<SpriteFrame *> getAnimation(std::string name);
		void touziCallBack();  // 2骰子动画调用
		void releaseSpriteFrameVector(cocos2d::Vector<SpriteFrame *>& vec);
	
		virtual void outCard(const sitDir& dir, const INT& number) override;
		virtual void catchCard(const sitDir& dir, const INT& number, const bool& head = true) override;
		virtual void setPlayGame(bool bs = true) override;

		virtual void reconnected(
			std::vector<std::vector<INT>>& vvHandCard, 
			std::vector<std::vector<INT>>& vvOutCard, 
			std::vector<std::vector<std::vector<INT>>>& vvvActionCard,
			std::vector<std::vector<INT>>& vvMenPai,
			std::vector<bool>& vTrusteeshep,
			std::vector<INT>& vQueMen,
			sitDir& ntDir) override;   // 断线重连
private:
	bool _autoBegin;	

	std::vector<std::vector<INT>> _vecStartSendCard;
	INT _touzi0;
	INT _touzi1;
	sitDir _startCatchDir;

	private:          // ui
		// 离开
		virtual void onGameDisconnect() override;

	private:  // 逻辑层
		GameTableLogic* _tableLogic;

	private:          //  核心数据
		GameManager* _mahjongManager;
	};

}
#endif
