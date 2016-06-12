#include "BJLGameLogic.h"
#include <stdlib.h>
#include <string.h>

namespace BJL
{
	//static BYTE	s_CardData[FULL_COUNT] =
//{
//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 2 - A
//	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 2 - A
//	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 2 - A
//	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 2 - A
//	0x4E,0x4F //小鬼，大鬼
//};

	static const WORD LOGIC_MASK_COLOR = 0x00F0;
	static const WORD LOGIC_MASK_VALUE = 0x000F;

	//////////////////////////////////////////////////////////////////////////
	
	GameLogic::GameLogic()
	{
	}

	GameLogic::~GameLogic()
	{
	}

	WORD GameLogic::GetCardValue(WORD cbCardData) { return cbCardData&LOGIC_MASK_VALUE;}
	
	WORD GameLogic::GetCardColor(WORD cbCardData) { return cbCardData&LOGIC_MASK_COLOR;}

	void GameLogic::RandCardList(WORD cbCardBuffer[], WORD cbBufferCount)
	{
	}

	WORD GameLogic::GetCardPip(WORD cbCardData)
	{
		WORD cbCardValue=GetCardValue(cbCardData);
		WORD cbPipCount=(cbCardValue>=10)?0:cbCardValue;

		return cbPipCount;
	}

	WORD GameLogic::GetCardListPip(const WORD cbCardData[], WORD cbCardCount)
	{
		WORD cbPipCount=0;

		for (WORD i=0;i<cbCardCount;i++)
		{
			cbPipCount=(GetCardPip(cbCardData[i])+cbPipCount)%10;
		}

		return cbPipCount;
	}



	int GameLogic::GetPoint(WORD wCard)
	{
		if (wCard == 0x00)
			return 0;
		int ans=GetCardValue(wCard);
		if (ans>10)
		{
			return 10;
		}
		else
		{
			return ans;
		}
	}

	///计算牌中点数
	///@param: WORD wCardList[] [in] 牌集
	///@param: int  iCardCount  [in] 牌张数
	///@return: int [out]  返回牌点数
	int GameLogic::CountPoint(WORD  wCardList[],int iCardCount)
	{
		int point = 0;
		for (int i=0;i<iCardCount;i++)
		{
			int temp = GetPoint(wCardList[i]);
			if (temp == 14)
				temp = 1;
			point += temp;
		}
		return point;
	}

	int GameLogic::CanSumIn(WORD wCardList[],int iCardCount,int iSelectNum)
	{
		int total = CountPoint(wCardList,iCardCount);
		for (int i=0; i<3; i++)
		{
			if (GetCardValue(wCardList[i]) == 0xE
				|| GetCardValue(wCardList[i]) == 0xF)
			{
				continue;
			}
			for (int j=i+1; j<4; j++)
			{
				if (GetCardValue(wCardList[j]) == 0xE
					|| GetCardValue(wCardList[j]) == 0xF)
				{
					continue;
				}

				for (int k = j+1; k<iCardCount; k++)
				{
					if (GetCardValue(wCardList[k]) == 0xE
						|| GetCardValue(wCardList[k]) == 0xF)
					{
						continue;
					}

					int temp = GetPoint(wCardList[i])+GetPoint(wCardList[j])+GetPoint(wCardList[k]);
					if (temp==30||temp==10||temp==20)
					{
						return total-temp;
					}
				}
			}
		}
		return -1;
	}



	///得到手牌中最大的牌(含花色)
	///@param: WORD wCardList[] [in] 牌集
	///@param: int  iCardCount  [in] 牌张数
	///@return: int [out]  最大牌
	int GameLogic::GetMaxCard(WORD  wCardList[],int iCardCount)
	{
		int temp = 0;
		BYTE card = 0;
		for (int i = 0; i < iCardCount; i++)
		{
			if (temp == 0)
			{
				temp = GetCardValue(wCardList[i]);
				card = wCardList[i];
			}
			else
			{
				if (temp < GetCardValue(wCardList[i]))
				{
					temp = GetCardValue(wCardList[i]);
					card =wCardList[i];
				}
	/*			else if (temp == m_pgamelogic->GetCardValue(wCardList[i]))
				{
					if(GetCardShape(card) < GetCardShape(wCardList[i]))
					{
						temp = m_pgamelogic->GetCardValue(wCardList[i]);
						card =wCardList[i];
					}
				
				}*/
			}
		}
		return card;
	}


	int  GameLogic::CountKing(WORD  wCardList[],int iCardCount)
	{
		int temp=0;
		for (int i=0; i<iCardCount; i++)
		{
			if (GetCardValue(wCardList[i]) == 0xE
				|| GetCardValue(wCardList[i]) == 0xF)
			{
				temp++;
			}
		}
		return temp;
	}

	bool GameLogic::IsBullBull(WORD  wCardList[],int iCardCount)
	{
		int total=CountPoint(wCardList,iCardCount);
		if(CanSumIn(wCardList,iCardCount,3)==-1)
			return false;
		if ((total > 0) && (total % 10 == 0))
			return true;
		return false;
	}

	int GameLogic::IsHaveNote(WORD  wCardList[],int iCardCount)
	{
		int Note = CanSumIn(wCardList, iCardCount, 3);
		return (Note % 10);
	}

	int GameLogic::GetMaxPoint(WORD  wCardList[],int iCardCount,WORD wBullCard[])
	{
		int iMaxPoint = 0;
		int iTempPoint= 0;
		for (int i=0; i<4; i++)
		{
			if (GetCardValue(wCardList[i]) == 0xE
				|| GetCardValue(wCardList[i]) == 0xF)
			{
				continue;
			}
			for (int j=i+1; j<5; j++)
			{
				if (GetCardValue(wCardList[j]) == 0xE
					|| GetCardValue(wCardList[j]) == 0xF)
				{
					continue;
				}
				iTempPoint = GetPoint(wCardList[j]) + GetPoint(wCardList[i]);
				if (iTempPoint%10 == 0)
				{
					iMaxPoint = iTempPoint%10;
					wBullCard[0] = wCardList[i];
					wBullCard[1] = wCardList[j];
					return 0;
				}
				else
					if (iTempPoint%10 > iMaxPoint)
					{
						iMaxPoint = iTempPoint%10;
						wBullCard[0] = wCardList[i];
						wBullCard[1] = wCardList[j];
					}
			}
		}
		return iMaxPoint;
	}

	///得到当前牌型
	///@param: WORD wCardList[] [in] 牌集
	///@param: int  iCardCount  [in] 牌张数
	///@return: int [out]  返回牌型
	int GameLogic::GetShape(WORD wCardList[],int iCardCount)
	{

		if (CanSumIn(wCardList, iCardCount, 3)>0 && GetCardValue(GetMaxCard(wCardList,iCardCount)) == 0x0E)
			return 1010;
		if (CanSumIn(wCardList, iCardCount, 3)>0 && GetCardValue(GetMaxCard(wCardList,iCardCount)) == 0x0F)
			return 1011;
		if (2 == CountKing(wCardList,iCardCount)) return 1010;
		if(IsBullBull(wCardList,iCardCount)) return 1012;
		switch(IsHaveNote(wCardList,iCardCount))
		{
		case 1:
			return 1001;
		case 2:
			return 1002;
		case 3:
			return 1003;
		case 4:
			return 1004;
		case 5:
			return 1005;
		case 6:
			return 1006;
		case 7:
			return 1007;
		case 8:
			return 1008;
		case 9:
			return 1009;

		}
		if (GetCardValue(GetMaxCard(wCardList,iCardCount)) == 0xE
			||GetCardValue(GetMaxCard(wCardList,iCardCount) == 0xF))//王代牛
		{
			WORD Temp[5] = {0};
			switch(GetMaxPoint(wCardList,iCardCount,Temp))
			{
			case 0:
				if (GetCardValue(GetMaxCard(wCardList,iCardCount)) == 0xE)
					return 1010;
				if (GetCardValue(GetMaxCard(wCardList,iCardCount)) == 0xF)
					return 1011;
				break;
			case 1:
				return 1001;
			case 2:
				return 1002;
			case 3:
				return 1003;
			case 4:
				return 1004;
			case 5:
				return 1005;
			case 6:
				return 1006;
			case 7:
				return 1007;
			case 8:
				return 1008;
			case 9:
				return 1009;
			}
		}
		return 1000;

	}

	//获取牛牛
	bool GameLogic::GetBull(WORD wCardList[],int iCardCount,WORD wBullCard[])
	{
		int total = CountPoint(wCardList,iCardCount);
		for (int i=0; i<3; i++)
		{
			if (GetCardValue(wCardList[i]) == 14
				|| GetCardValue(wCardList[i]) == 15)
			{
				continue;
			}
			for (int j=i+1; j<4; j++)
			{
				if (GetCardValue(wCardList[j]) == 14
					|| GetCardValue(wCardList[j]) == 15)
				{
					continue;
				}
				for (int k = j+1; k<iCardCount; k++)
				{
					if (GetCardValue(wCardList[k]) == 1
						|| GetCardValue(wCardList[k]) == 15)
					{
						continue;
					}
					int temp = GetPoint(wCardList[i])+GetPoint(wCardList[j])+GetPoint(wCardList[k]);
					if (temp==30||temp==10||temp==20)
					{
						wBullCard[0]=wCardList[i];
						wBullCard[1]=wCardList[j];
						wBullCard[2]=wCardList[k];
						return true;
					}
				}
			}
		}
		if (2 == CountKing(wCardList,iCardCount)) //双王一定是牛牛
		{
			int index = 0;
			int kingCount = 0;
			for (int k=0; k<5; k++)
			{
				if (GetCardValue(wCardList[k]) == 15
					&& kingCount == 0)
				{
					kingCount++;
					wBullCard[index++] = wCardList[k];
				}
				if (2 == index && 0 == kingCount)
				{
					continue;
				}
				else
				{
					if (GetCardValue(wCardList[k]) != 14
						&& GetCardValue(wCardList[k]) != 15)
					{
						wBullCard[index++] = wCardList[k];
					}
				}
				if (index == 3 && kingCount == 1)
				{
					return true;
				}
			}
		}
		if (GetCardValue(GetMaxCard(wCardList,iCardCount)) == 14
			||GetCardValue(GetMaxCard(wCardList,iCardCount) == 15))//王代牛
		{
			int iMaxPoint = 0;
			int iTempPoint= 0;
			int index = 0;
			for (int i=0; i<4; i++)
			{
				if (GetCardValue(wCardList[i]) == 14
					|| GetCardValue(wCardList[i]) == 15)
				{
					continue;
				}
				for (int j=i+1; j<5; j++)
				{
					if (GetCardValue(wCardList[j]) == 14
						|| GetCardValue(wCardList[j]) == 15)
					{
						continue;
					}
					iTempPoint = GetPoint(wCardList[j]) + GetPoint(wCardList[i]);
					if (iTempPoint%10 == 0)
					{
						iMaxPoint = iTempPoint%10;
						index = 0;
						for (int k=0; k<5; k++)
						{
							if (k!=i && k!= j)
							{
								wBullCard[index++] = wCardList[k];
							}
							if (index == 3)
							{
								return true;
							}						
						}
					}
					else
						if (iTempPoint%10 > iMaxPoint)
						{
							iMaxPoint = iTempPoint%10;
							index = 0;
							for (int k=0; k<5; k++)
							{
								if (k!=i && k!= j)
								{
									wBullCard[index++] = wCardList[k];
								}
								if (index == 3)
								{
									break;
								}
							}
						}
				}
			}
			if (index == 3)
			{
				return true;
			}
		}
		return false;
	}
}