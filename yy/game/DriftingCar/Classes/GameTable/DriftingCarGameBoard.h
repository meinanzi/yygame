#ifndef __DriftingCarGameBoard_H__
#define __DriftingCarGameBoard_H__

/*
 *	跑马灯板子
 */

#include "HNUIExport.h"
#include "HNNetExport.h"
#include <array>
#include "DriftingCarMessageHead.h"

namespace DriftingCar
{
	class GameCell;

	class GameBoard:public HN::HNLayer
	{
	public:

		typedef std::function<void (cocos2d::Node*)> PlayCallBack;//跑车跑完时，回调函数

		typedef std::function<void (int)>CallCarAnimation;

		static GameBoard* create(Node* pNode);

		virtual bool init(Node* pNode);

		void CarPlay(float);

		void startPlay( int endIndex, BYTE btime);			//播放动画

		void HideCarAndAllCell();							//隐藏跑车和所有跑灯

		void ShowCarBlinkLogos(int index);					//指定索引的跑灯闪烁

		void showCarStart();								//车移动到指定开始位置

		PlayCallBack CallBackPlayAnimation;					//动画播放完的回调函数

		CallCarAnimation CallBackCarIndex;					//跑车在跑时，返回的索引
		//
	protected:
		static float speed_level1;
		static float speed_level2;
		static float speed_level3;
		enum 
		{
			LEVEL_COUNT = 3,
			CAR_LOGO_TYPE = 8,//种类
		};
		GameBoard();
		~GameBoard();
		void loadCells(Node* layout);
		void switchCell(float delta);
		void InitCarAnimation();			//初始化车动作

		void showAction(int index, float fduration);
		void HideAction(int index);

		
		
		void InitCarCoord();					//修正车子坐标系
		void SetCarStartPosition();				//设置car回到开始位置
		
		void StartPlayOther(int endIndex, int btime);					//另外一种跑车动画实现方式

	private:
		std::vector<GameCell*> _cells;		

		std::array<Vec2,CAR_LOGO_COUNT> _Vec2Car;

		Vector<FiniteTimeAction*> _vtAction;

		Sprite* _spriteCar;						//车精灵

		int _startIndex;
		int _endIndex;
		int _currentIndex;
		float _totalTime;
		float _runTime;
		int _level1Count;
		int _level2Count;
		int _level3Count;
		float _levelTime[LEVEL_COUNT];//20%,70%,10%

		float _jiashudu;				//加速度
		int _iRoundcount;				//移动步数
		int _iSumCount;					//总步数
	};
}

#endif // __GuessCarGameBoard_H__
