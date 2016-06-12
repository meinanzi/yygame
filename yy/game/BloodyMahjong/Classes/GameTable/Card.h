#ifndef _CARD_H_
#define _CARD_H_

#include "cocos2d.h"
#include "XZMJ_MessageHead.h"
#include "ResourceLoader.h"

namespace XZMJ
{

#define CREATE_CARD(__className) \
	static __className * create(mahjongCreateType type,   sitDir sitNo, INT number = -999) \
	{ \
	auto p = new __className; \
	if (p && p->init(type, sitNo, number)) \
	{ \
	p->autorelease(); \
	return p; \
	} \
	delete p; \
	return nullptr; \
}

	class Card :
		public cocos2d::Node
	{
	public:
		Card(void);
		~Card(void);

		virtual bool init(mahjongCreateType _type, sitDir sitNo, INT number) = 0;         // 初始化卡牌的属性
	
		// set 属性
		void setCardPos(const Vec2& pos);																	// 设置卡牌的位置
		void setCardZorder(const int zorder);																// 设置卡牌的localZorder
		void setCardVisible(const bool& bs = true);														// 设置卡牌显示
		void setCardOwner(const sitDir& dir);																// 设置卡牌的使用者
		void setCardEnableTouch(bool touch = true);                                                   // 设置卡牌的触摸开关
		void setCardTouchEvent();                                                                                // 设置卡牌的触摸属性

		// get 属性
		const Size& getCardSize();																				// 获取卡牌的尺寸
		Sprite* getCardSprite();																						// 获取卡牌的精灵
		const Vec2& getCardPos() { return _uiSp->getPosition(); }								    // 获取卡牌的位置
		bool isCardVisible();																							// 获取卡牌是否显示
		const sitDir& getCardOwner() { return _sitNo; };													// 获取卡牌的使用者
		const INT& getCardNumber();																			// 获取卡牌的点数
		const mahjongColor& getCardColor();															    // 获取卡牌的花色
		const INT& getCardZorder();                                                                            // 获取卡牌的localZoder值
		const INT& getCardSumNumber() { return _sum; }                                           // 获取卡牌的编码值

	protected:
		cocos2d::Sprite* _uiSp;																						// 界面显示
		mahjongCreateType _type;																				// 创建类型
		INT _number;																									// 点数
		sitDir _sitNo;																										// 使用者
		mahjongColor _color;																						// 花色    
		Size _size;																											// 尺寸   
		bool _canTouch;																								// 触摸
		INT _cardZorder;																								// localZoder
		INT _sum;                                                                                                         // 编码值
		Vec2 _startPos;																									// 起始坐标位置
		INT _startZorder;																								// 起始zorder值
		Color3B _startColor;																							// 起始颜色
	};

}
#endif // _CARD_H_