#ifndef __BJLResultCard_h__
#define __BJLResultCard_h__

#include "HNNetExport.h"
#include "HNUIExport.h"

#include "BJLMessageHead.h"
#include "BJLGameLogic.h"
#include "BJLPokerCard.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>

using namespace cocostudio;
using namespace ui;
using namespace std;

namespace BJL
{
	class GameTableUI;

	class ResultCard
	{
	public:
		ResultCard(GameTableUI* gameTableUi);
		~ResultCard();

	public:
		bool dispatchCard();
		bool dispatchCardWithoutAnimation();
		void showCardBorad();
		void showCardBoradWithOutAnimation();
		void calculatePoint();								//计算游戏牌型点数
		void hideCardBorad();

		// 获取数据
		void initCard(BYTE card[2][3], BYTE dealerCardNum, BYTE leisureCardNum,int dealerCardtype[5],int leisureCardtype[5] );
		void initNewGame();

	public:
		BYTE getCardFileValue(const WORD& card);


	private:
		GameTableUI* _gameTableUi;
		Node* _csLoader;
		Node* _panel;
		PokerCard* _spriteCard[2][3];
		ImageView* background;
		ImageView* leisure;
		ImageView* dealer;
		ImageView* leisure_win;
		ImageView* dealer_win;
		ImageView* dealer_leisure;
		
		//牌堆图片
		ImageView* cardsss;

		ImageView* cardTest[2];		//牌图片 ，用于播放动画时确定位子  手动没调好，  0.0

	private:
		BYTE _cards[2][3];				   //牌值
		Vec2 destinationPositon[2][3];     //目标位置
		int _dealerCardtype[5];			   //庄家牌型
		int _leisureCardtype[5];           //闲家牌型
		BYTE    _bSendCardIndex;            //发牌索引
		BYTE _dealerCardNum;
		BYTE _leisureCardNum;
		Sprite* spArr[2];
	private:
		void cardFlipCallback(Node* pNode);
		void dispatchCardAnimation(PokerCard* pokerCard , BYTE state);
		BYTE countTheAnswer();
	};
}

#endif // __BJLResultCard_h__
