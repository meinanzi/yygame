#ifndef __TRACTORGameLogic_h__
#define __TRACTORGameLogic_h__


#include "HNBaseType.h"
namespace TRACTOR
{
	//////////////////////////////////////////////////////////////////////////////////
	enum
	{
		//数值掩码
		LOGIC_MASK_COLOR	=		0xF0	,							//花色掩码
		LOGIC_MASK_VALUE	=		0x0F	,							//数值掩码

		//扑克类型
		CT_SINGLE			=		1		,							//单牌类型
		CT_ONE_LONG		=		2		,							//对子类型
		CT_TWO_LONG		=		3		,							//两对类型
		CT_THREE_TIAO		=		4		,							//三条类型
		CT_SHUN_ZI		=		5		,							//顺子类型
		CT_TONG_HUA		=		6		,							//同花类型
		CT_HU_LU			=		7		,							//葫芦类型
		CT_TIE_ZHI			=		8		,							//铁支类型
		CT_TONG_HUA_SHUN	=		9		,							//同花顺型
		CT_KING_TONG_HUA_SHUN	=	10		,							//皇家同花顺

		//////////////////////////////////////////////////////////////////////////////////

		GAME_PLAYER			=	8		,							//玩家个数
		MAX_CENTERCOUNT		=	5		,							//最大数目
		FULL_COUNT				=	52		,							//全牌数目
		MAX_COUNT				=	2		,							//最大数目
	};

	//胜利信息结构
	struct UserWinList
	{
		BYTE bSameCount;
		WORD wWinerList[GAME_PLAYER];
	};

	//分析结构
	struct CardAnalyseResult
	{
		BYTE 							cbFourCount;						//四张数目
		BYTE 							cbThreeCount;						//三张数目
		BYTE 							cbLONGCount;						//两张数目
		BYTE							cbSignedCount;						//单张数目
		BYTE 							cbFourLogicVolue[1];				//四张列表
		BYTE 							cbThreeLogicVolue[1];				//三张列表
		BYTE 							cbLONGLogicVolue[2];				//两张列表
		BYTE 							cbSignedLogicVolue[5];				//单张列表
		BYTE							cbFourCardData[MAX_CENTERCOUNT];	//四张列表
		BYTE							cbThreeCardData[MAX_CENTERCOUNT];	//三张列表
		BYTE							cbLONGCardData[MAX_CENTERCOUNT];	//两张列表
		BYTE							cbSignedCardData[MAX_CENTERCOUNT];	//单张数目
	};
	//////////////////////////////////////////////////////////////////////////////////

	// 游戏逻辑
	class GameLogic
	{
		//函数定义
	public:
		GameLogic();
		~GameLogic();

		//控制函数
	public:
		//排列扑克
		void sortCard(BYTE cbCardData[], BYTE cbCardCount);
		//混乱扑克
		void randCard(BYTE cbCardBuffer[], BYTE cbBufferCount);

		//类型函数
	public:
		//获取类型
		BYTE getCardType(BYTE cbCardData[], BYTE cbCardCount);
		//获取数值
		BYTE getCardValue(BYTE cbCardData) { return cbCardData & LOGIC_MASK_VALUE; }
		//获取花色
		BYTE getCardColor(BYTE cbCardData) { return cbCardData & LOGIC_MASK_COLOR; }
		//查找扑克
		BYTE getSameCard(const BYTE bCardData[], const BYTE bMaxCard[], BYTE bCardCount, BYTE bMaxCardCount, BYTE bResultData[]);

		//功能函数
	public:
		//逻辑数值
		BYTE getCardLogicValue(BYTE cbCardData);
		//对比扑克
		BYTE compareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);
		//分析扑克
		void analysebCardData(const BYTE cbCardData[], BYTE cbCardCount, CardAnalyseResult & AnalyseResult);
		//7返5
		BYTE fiveFromSeven(BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbCenterCardData[],BYTE cbCenterCardCount,BYTE cbLastCardData[],BYTE cbLastCardCount);
		//查找最大
		bool selectMaxUser(BYTE bCardData[GAME_PLAYER][MAX_CENTERCOUNT],UserWinList &EndResult,const INT lAddScore[]);

	};
}

//////////////////////////////////////////////////////////////////////////


#endif // __TRACTORGameLogic_h__