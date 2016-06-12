#ifndef _SOUTHMAHJONGCARDPOOL_H_
#define _SOUTHMAHJONGCARDPOOL_H_

#include "MahjongCardPool.h"

namespace XZMJ
{
	class SouthMahjongCardPool :
		public MahjongCardPool
	{
	public:
		SouthMahjongCardPool(void);
		~SouthMahjongCardPool(void);

		CREATE_COUNT(SouthMahjongCardPool);
		virtual bool init(INT count);

		virtual void setHandCardPos(Card* noCard = nullptr) override;						// 安置手牌
		virtual void sortCard() override;									// 整理手牌
		virtual void removeCardMsg(Card* card) override;	
		virtual void removeCard() override;
		void removeCardMsgCallBack();  
		virtual void setCanOper(bool bs = true);                        // 设置能否操作牌
		virtual Card* getAutoSendCard() override;  // 取自动出牌的手牌
		bool hasNoQueColor();  // 无缺门花色
		bool checkTouchCard(Card* card); // 检测是否可碰
		bool checkMeldedKong(Card* card);  // 检测明杠
		bool checkConcealedKong();  // 检测暗杠
		bool checkTouchKong(Card* card);  // 检测碰杠
		virtual void afterAction(const INT& removeCount) override;   
		virtual void afterCatchCheckSomething(Card* card) override;  // 抓牌检测
		virtual void afterOutCheckSomething(Card* card) override;               // 出牌检测
		void sortListCard(std::list<Card *>& list);
		bool canTing();
		bool canHu(HUPAI_TYPE_EX& typeBase, HUPAI_TYPE_EX& typeAdd, bool& hasAdd, Card* card = nullptr);

	private:
		void sendBlockMsg();
		void canBlock();
		bool isShunZi(std::list<INT>& listNum);  // 组成顺子
		void calcCardCount(std::list<Card *>& listCard,std::list<INT>& listCardNum, std::vector<INT>& vecCard, std::vector<INT>& vCount);
		void calCOCOS_NODEumCount(std::list<INT>& listCardNum,std::vector<INT>& vecCard, std::vector<INT>& vCount);
		// 胡牌检测
		bool isPingHu(Card* card = nullptr);   // 平胡  
		bool isDuiDuiHu(Card* card = nullptr); // 对对胡
		bool isQingYiSe(Card* card = nullptr);  // 清一色
		bool isDaiJiuYao(Card* card = nullptr);  // 带幺九
		bool isXiaoQiDui(Card* card = nullptr);    // 小七对
		bool isJiangDui(Card* card = nullptr);   // 将对
		bool isQingDui(Card* card = nullptr);    // 清对
		bool isLongQiDui(Card* card = nullptr);  // 龙七对
		bool isQingQiDui(Card* card = nullptr);  // 清七对
		bool isQingYaoJiu(Card* card = nullptr); // 清幺九
		bool isLongQingQiDui(Card* card = nullptr);  // 龙清七对
		bool isYaoJiuQiDui(Card* card = nullptr);    // 幺九七对
		bool isTianHu(Card* card = nullptr); // 天胡
		bool isDiHu(Card* card = nullptr);     // 地胡


	private:
		mahjongColor _queColor; // 缺门
		Card* _willRemoveCard; // 将移除的牌
		bool _hasSelectOutCard;
	};

}

#endif