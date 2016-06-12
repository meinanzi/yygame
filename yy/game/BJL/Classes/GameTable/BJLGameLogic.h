#ifndef __BJLGameLogic_h__
#define __BJLGameLogic_h__

namespace BJL
{

	typedef unsigned char   BYTE;
	typedef unsigned short	WORD;

	class GameLogic
	{
	private:

	public:
		GameLogic();
		virtual ~GameLogic();

	public:
		WORD GetCardValue(WORD cbCardData);
		WORD GetCardColor(WORD cbCardData);

	public:
		void RandCardList(WORD cbCardBuffer[], WORD cbBufferCount);

	public:
		//获取单张牌的点数
		WORD GetCardPip(WORD cbCardData);
		//获取总的点数
		WORD GetCardListPip(const WORD cbCardData[], WORD cbCardCount);

		///得到当前牌型
		///@param: WORD wCardList[] [in] 牌集
		///@param: int  iCardCount  [in] 牌张数
		///@return: int [out]  返回牌型
		int GetShape(WORD wCardList[],int iCardCount);
		//任意三张牌能否取整
		int CanSumIn(WORD wCardList[],int iCardCount,int iSelectNum);
		//点数和
		int CountPoint(WORD  wCardList[],int iCardCount);
		int GetPoint(WORD wCard);
		//获取最大牌值
		int GetMaxCard(WORD  wCardList[],int iCardCount);
		int CountKing(WORD  wCardList[],int iCardCount);
		bool IsBullBull(WORD  wCardList[],int iCardCount);
		int IsHaveNote(WORD  wCardList[],int iCardCount);
		//%10 == 0 时返回0  无0返回最大的
		int GetMaxPoint(WORD  wCardList[],int iCardCount,WORD wBullCard[]);
		//获取牛牛
		bool GetBull(WORD wCardList[],int iCardCount,WORD wBullCard[]);
	};

	//////////////////////////////////////////////////////////////////////////

}

#endif // __BJLGameLogic_h__