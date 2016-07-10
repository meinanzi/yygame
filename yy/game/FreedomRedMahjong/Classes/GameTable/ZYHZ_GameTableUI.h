#ifndef _ZYHZ_GAMETABLE_H
#define _ZYHZ_GAMETABLE_H

/*****************************************************/
// hn.h
#include "ZYHZ_MessageHead.h"
#include "ZYHZ_GameTableUICallback.h"
#include "ZYHZ_GameTableLogic.h"
#include "ZYHZ_GameManager.h"


/*****************************************************/

namespace ZYHZ
{
	// namespace
	USING_NS_CC;
	using namespace HN;
	using namespace ui;
	using namespace std;

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
        
        // 买码
        virtual void showMaimaOption(BYTE timer, tagMaiMa* data) override;
        virtual void hideMaimaOption() override;
        std::string getMjSpriteframeName(mahjongCreateType, sitDir, int);
private:
        int _maimaCount;
        tagMaiMa _maimaData;
        int _timer;
        Node* _maimaNode;
        
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
