#ifndef _ZYHZ_CARDPOOL_H_
#define _ZYHZ_CARDPOOL_H_

#include "ZYHZ_Card.h"

namespace ZYHZ
{
	class PoolAction;

	class CardPool :
		public cocos2d::Node
	{
	public:
		CardPool(void);
		~CardPool(void);

		virtual void addCard(Card* card);            // 从牌堆里摸一张牌
		virtual void removeCardMsg(Card* card) {};
		virtual void removeCard();      // 打出一张牌
		virtual void sendSomeCard(std::vector<Card *> vec);     // 开场发一些牌
		virtual void sortCard() {};                        // 整理牌堆
		virtual bool init() override;       
		virtual void setCatchPos(const Vec2& pos) { _catchPos = pos; _outToDeskPos = pos; };   // 设置摸的牌摆放初始位置和出牌位置
		void setStartSortPos(const Vec2& pos) { _startSortPos = pos; }
		virtual void setHandCardPos(Card* noCard = nullptr) {};  // 安置手牌
		virtual Card* getAutoSendCard();  // 取自动出牌的手牌
		const Vec2& getOutToDeskPos() { return _outToDeskPos; }  // 出牌桌面位置
		virtual void setCanOper(bool bs = true) {};  // 可触摸
		list<Card *> & getListCard() { return _listCard; }   // 链表
		virtual void setPoolAction(PoolAction* action);    // 设置动作
		void addLeftOutCount(const INT& count);   // 增加左边倒牌
		const INT& getLeftOutCount();                    // 倒牌数
		const sitDir& getSitDir();                               // 方向
		const Vec2& getStartSortPos() { return _startSortPos; }        // 开始摆放位置
		const Vec2& getCurrHandCardStartSortPos() { return _currSortStartPos; }
		virtual void afterCatchCheckSomething(Card* card) {};		// 抓牌检测动作
		virtual void afterOutCheckSomething(Card* card) {};		// 出牌检测动作

		virtual void addSomeOutCards(const INT& count, const INT& number, bool hideLastOutCard = true, bool isMingGang = true); // 碰牌
		const Vec2& getActionPos();    // 播放动画位置i

		virtual void afterAction(const INT& removeCount);

		void setActionCardNumber(const INT& number) { _actionNumber = number; };    // 动作牌
		void setTouchActionVector(const std::vector<Card *> vecCard) {_vecTouchCard = vecCard; };  // 设置碰牌动作牌
		std::vector<Card *>& getTouchVec() { return _vecTouchCard; };

		void setOutCardCount(const INT& count) { _sendCardCount = count; }
		const INT& getOutCardCount() { return _sendCardCount; }
		virtual void huCard(bool isZimo = false, bool isQiangGang = false);  // 胡牌
		virtual void finishGame();

		std::list<Card *>& getHuListCard() { return _listHuCard; }  // 胡牌列表
		bool getHasHu() { return _hasHuCard; }  // 是否已经胡了
		void setHasHu(const bool& bs = true) { _hasHuCard = bs; }

		void reSetData();  // 重置数据，新局开始
		void setCanCheckAction() { _needCheckPeng = true; _needCheckHu = true; };  // 设置可接受动作
		void setLastActionCardNumber(const INT& number) { _lastActionCardNumber = number; }   // 设置胡的牌
		const INT& getLastActionCardNumber() { return _lastActionCardNumber; }   // 获取胡的牌
		float getSortInterval() { return _sortIntervel; }
		float getActionCardInterval() { return _actionCardIntervel; }

	protected:
		std::list<Card *> _listCard;  // 卡片链表
		std::list<Card *>::iterator _listIterStart;   // list迭代器
		std::list<Card *>::iterator _listIterEnd;
		bool _isInSort;   // 排列模式
		cocos2d::Vec2 _catchPos;  // 抓牌位置
		cocos2d::Vec2 _startSortPos;
		cocos2d::Vec2 _currSortStartPos;  // 当前手牌起始点
		float _sortIntervel;		// 手牌排列间隔
		float _actionCardIntervel;  // 动作拍间隔
		float _outCardInterval;
		Vec2 _outToDeskPos; // 出牌位置
		PoolAction* _poolAction;   // 打牌动作
		sitDir _dir;                    // 方位
		INT _leftCount;      // 已经出的牌数目
		Size _cardSize; // 桌面手牌尺寸
		INT _actionNumber;  // 操作的编码
		Card* _rbeginActionCard;  // 最右手边动作牌
		Vec2 _actionCardMoveV;     // 动作牌偏移量 水平
		Vec2 _actionCardMoveT;		// 动作牌偏移量 竖直

		std::vector<Card *> _vecTouchCard; // 碰牌的容器
		std::vector<Card *> _vecKongCard; //  杠牌容器
		std::vector<Card *> _vecTmpKongCard;  // 暗杠临时牌

		INT _sendCardCount;  // 出牌数
		bool _hasHuCard;   // 是否已经胡牌
		INT _lastActionCardNumber;    // 最后的动作牌

		std::list<Card *> _listHuCard;   // 胡牌的手牌

	public:
		void setNotCheckHu() { _needCheckHu = false; };
		void setNotCheckPeng() { _needCheckPeng = false; }
		const bool& getCanPeng() { return _checkedCanPeng; }
		const bool& getCanHu() { return _checkedCanHu; }

	protected:
		bool _needCheckHu; // 检测是否可胡动作
		bool _needCheckPeng; // 可碰
		bool _checkedCanHu;  // 检测到能胡
		bool _checkedCanPeng; // 检测到能碰
	};

}

#endif // _CARDPOOL_H_
