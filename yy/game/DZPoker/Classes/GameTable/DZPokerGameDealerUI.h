#ifndef __DZPokerGameDealerUI_h__
#define __DZPokerGameDealerUI_h__

#include "HNNetExport.h"
#include <array>
#include "DZPokerGameChipUI.h"
#include "DZPokerUpgradeMessage.h"
#include "DZPokerGameClientMessage.h"

/*
 * 荷官类，专门处理荷官处理的事物
 */

namespace DZPoker
{
	class GameDelearUI: public HN::HNLayer
	{
	public:
		/*创建荷官*/
		static GameDelearUI* create();
		/*设置荷官位置（初始化一次）*/
		void setDealerPoint(cocos2d::Vec2 point);
		/*设置牌桌上玩家的点（初始化一次）*/
		void setTablePoint(std::vector<cocos2d::Vec2>& points);
		/*设置玩家座位的点（初始化一次）*/
		void setSeatPoint(std::vector<cocos2d::Vec2>& points);
		/*设置庄家位置（庄家变化就需要修改）*/
		void setButtonStation(BYTE buttonStation);
		/*设置公共牌位置*/
		void setCommunityPoint(const std::vector<cocos2d::Vec2>& points);
	
		//玩家下注
		void betMoney(BYTE seatNo, LLONG money);
		//移动庄家
		void dealButton(BYTE dealerStation);
		//给玩家发手牌
		void dealHandCard(const std::vector<THandCard>& handCard);
		//发翻牌
		void dealFlopCard(const std::vector<BYTE>& flopCard);
		//发转牌
		void dealTurnCard(BYTE turnCard);
		//发河牌
		void dealRiverCard(BYTE riverCard);
		//收牌
		void dealCollectCard();
		/* 给玩家派奖（先收筹码，再派奖）
		 * 数组值非零表示有奖
		 */
		void dealWinPool(const std::vector<std::vector<LLONG>> &winPool);
		/*清理*/
		void clear();
		/* play player fold animation.
		 * @param seatNo player seatNo in view.
		 */
		void dealFold(BYTE seatNo);
	public:
		bool init();
	protected:
		GameDelearUI();
		virtual ~GameDelearUI();
		cocos2d::Vec2 getButtonPoint();
		cocos2d::Vec2 getSeatPoint(BYTE seatNo);
		cocos2d::Vec2 getTablePoint(BYTE seatNo);
		void collectChip();
		void sendPrize();
		void checkBet(cocos2d::Node* pNode);
		void dispatchHandCard(float delta);
		void showUserHandCard(Node* pSender);
		void moveFlopCard();
		void dispatchCommunityCard(int type);
		void openCommunityCard(cocos2d::Node* pNode);
		cocos2d::Action* getFlopAction();
		cocos2d::Action* getTurnAction();
		cocos2d::Action* getRiverAction();
	private:
		std::vector<cocos2d::Node*> _communityCard;//公共牌
		std::vector<cocos2d::Vec2> _communityCardPoints;//公共牌位置
		std::vector<GameChipUI*> _tableChip;//每个玩家桌上的筹码
		std::vector<cocos2d::Vec2> _seatPoints;//每个玩家座位上的点
		std::vector<cocos2d::Vec2> _tablePoints;//每个玩家桌上的点
		cocos2d::Vec2 _dealerPoint;//荷官的位置位置
		std::vector<std::vector<LLONG>> _winPool;//玩家赢的结果
		std::vector<THandCard> _handCard;//手上有牌的玩家座位号
		BYTE _curCount;
	private:
		cocos2d::Node* _dealer;
		BYTE _buttonStation;//庄家位置号
	
		bool seatNoOk(BYTE seatNo);

	public:
		std::function<void()> _sendCardFinish;
		// 发牌结束的回调
		void setSendCardFinish(const std::function<void()>& sendCardFinish);

	};

}

#endif // __DZPokerGameDealerUI_h__
