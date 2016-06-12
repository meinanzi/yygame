#ifndef __DriftingCarGameResultHistory_H__
#define __DriftingCarGameResultHistory_H__

#include "HNUIExport.h"
#include "HNNetExport.h"
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include <array>
namespace DriftingCar
{
	class GameResultHistory: public HN::HNLayer
	{
	public:
		static GameResultHistory* create();
		bool init();

		void LoadLayout(Node* playout);

		void addRecord(const std::vector<BYTE>& records, BYTE byResult);

		void ShowRecord();									//显示记录
		void ShowLastRecord();								//显示最后一页数据
		void ShowProbability();								//显示统计概率

		void ClickLeft();									
		void ClickRight();

	private:

		void ButtonLeft(Ref* pRef,Widget::TouchEventType TouchType);
		void ButtonRight(Ref* pRef, Widget::TouchEventType TouchType);

		void CheckRecordIndex();

		int GetMinIndex();
		int GetMaxIndex();

	protected:
		enum 
		{
			MAX_COUNT = 9,
			AreaCount = 8,
		};
		GameResultHistory();
		virtual ~GameResultHistory();
	private:


		std::array<Text*, AreaCount> _arrayText;				//概率文本

		std::array<ImageView*, MAX_COUNT> _arrayBigLogos;		//大图标
		std::array<ImageView*, MAX_COUNT> _arraySmallLogos;		//小图标
		std::array<ImageView*, MAX_COUNT> _arrayResult;			//出奖结果

		Button* _buttonLeft;									//左边按钮
		Button* _buttonRight;									//右边按钮

		std::vector<BYTE> vtByhistory;							//历史记录信息
		std::deque<BYTE> dequeReuslt;
		int _historyIndex;										//路子图的下标索引
	};
}

#endif // __GuessCarGameResultHistory_H__
