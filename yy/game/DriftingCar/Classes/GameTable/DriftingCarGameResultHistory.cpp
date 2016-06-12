#include "DriftingCarGameResultHistory.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define History_logo "DriftingCar/CardUi/Res/history/%ld.png"


namespace DriftingCar
{
	GameResultHistory* GameResultHistory::create()
	{
		GameResultHistory* layer = new GameResultHistory();
		if(layer->init())
		{
			layer->ignoreAnchorPointForPosition(false);
			layer->autorelease();
			return layer;
		}
		CC_SAFE_DELETE(layer);
		return nullptr;
	}

	bool GameResultHistory::init()
	{
		if(!HN::HNLayer::init())
		{
			return false;
		}

		return true;
	}

	void GameResultHistory::addRecord(const std::vector<BYTE>& records, BYTE byResult)
	{
		vtByhistory.clear();

		for (int i = 0; i < records.size(); i++)
		{
			if(records[i] <= AreaCount - 1)
			{
				vtByhistory.push_back(records[i]);
			}
		}
		if(vtByhistory.empty())
		{
			return;
		}
		//先判断结果记录是否为空
		if(dequeReuslt.size() == 0)
		{
			dequeReuslt.assign(vtByhistory.size() - 1, 0);	
		}

		dequeReuslt.push_back(byResult);//记录最新

		if(dequeReuslt.size() > 72)
		{
			dequeReuslt.pop_front();
		}

		ShowLastRecord();
	}

	GameResultHistory::GameResultHistory()
	{

		_buttonLeft = nullptr;
		_buttonRight = nullptr;
		_arrayResult.fill(nullptr);
		_historyIndex = 0;
	}

	GameResultHistory::~GameResultHistory()
	{

	}

	void GameResultHistory::ShowRecord()
	{
		//big 0123,small 4567
		CheckRecordIndex();

		int istart = _historyIndex * MAX_COUNT;

		int iCount = istart + MAX_COUNT;

		int iend = vtByhistory.size();


		for (int index = 0; index < MAX_COUNT; index++,istart++)
		{
			_arrayBigLogos[index]->setVisible(false);
			_arraySmallLogos[index]->setVisible(false);
			_arrayResult[index]->setVisible(false);

			if(istart < iend && istart < iCount)//如果先到尾部，先结束
			{
				const BYTE& by = vtByhistory[istart];
				char tm[40]={0};
				sprintf(tm,History_logo,by+1);

				bool bShowBig = true;

				switch(by)
				{
				case 0:case 2:case 4:case 6:
					{
						_arrayBigLogos[index]->loadTexture(tm);//大LOGO
						break;
					}
				case 1:case 3: case 5: case 7:
					{
						bShowBig =false;

						_arraySmallLogos[index]->loadTexture(tm);

						break;
					}
				}

				_arrayBigLogos[index]->setVisible(bShowBig);
				_arraySmallLogos[index]->setVisible(!bShowBig);

				//////////////////////////////////////////////////////////////////////////

				if(istart >= dequeReuslt.size())
					continue;

				sprintf(tm,"DriftingCar/CardUi/Res/%d.png",dequeReuslt[istart]);

				//得到坐标位置
				if(bShowBig)
				{
					_arrayResult[index]->setPosition(_arrayBigLogos[index]->getPosition());
				}
				else
				{
					_arrayResult[index]->setPosition(_arraySmallLogos[index]->getPosition());
				}
				//是否显示
				if(dequeReuslt[istart] != 0)
				{
					_arrayResult[index]->loadTexture(tm);
					_arrayResult[index]->setVisible(true);
				}
			}

		}
	}

	void GameResultHistory::ClickLeft()
	{
		--_historyIndex;

		ShowRecord();
	}

	void GameResultHistory::ClickRight()
	{
		
		++_historyIndex;

		ShowRecord();
	}

	void GameResultHistory::ButtonLeft( Ref* pRef,Widget::TouchEventType TouchType )
	{
		if(TouchType != Widget::TouchEventType::ENDED)
			return;

		ClickLeft();
	}

	void GameResultHistory::ButtonRight( Ref* pRef, Widget::TouchEventType TouchType )
	{
		if(TouchType != Widget::TouchEventType::ENDED)
			return;

		ClickRight();
	}

	void GameResultHistory::LoadLayout( Node* pLayout )
	{
		auto pPanel =  pLayout->getChildByName("Panel_BetArea");

		//概率分析
		for (int index = 0; index < AreaCount; index++)
		{
			char tm[40] = {0};
			sprintf(tm, "Text_%ld", index + 1);

			_arrayText[index] = (Text*)pPanel->getChildByName(tm);

			_arrayText[index]->setString("");

		}
		const int logoWidth = 54;
		const int logoHeight = 30;

		_buttonLeft = (Button*)pPanel->getChildByName("Button_left");
		_buttonRight = (Button*)pPanel->getChildByName("Button_right");

		_buttonLeft->addTouchEventListener(CC_CALLBACK_2(GameResultHistory::ButtonLeft, this));
		_buttonRight->addTouchEventListener(CC_CALLBACK_2(GameResultHistory::ButtonRight, this));


		ImageView* smallImage = (ImageView*)pPanel->getChildByName("Image_small");
		ImageView* bigImage = (ImageView*) pPanel->getChildByName("Image_big");

		smallImage->setVisible(false);
		bigImage->setVisible(false);

		Vec2 smallc2 = smallImage->getPosition();
		Vec2 bigc2	 = bigImage->getPosition();

		//历史图标
		for (int index = 0; index < MAX_COUNT; ++index)
		{
			int ix = smallc2.x + logoWidth* index;

			_arrayBigLogos[index] = ImageView::create();
			_arrayBigLogos[index]->setAnchorPoint(bigImage->getAnchorPoint());
			_arrayBigLogos[index]->setPosition(Vec2(ix, bigc2.y));

			_arraySmallLogos[index] = ImageView::create();
			_arraySmallLogos[index]->setAnchorPoint(smallImage->getAnchorPoint());
			_arraySmallLogos[index]->setPosition(Vec2(ix, smallc2.y));

			_arrayResult[index] = ImageView::create();
			_arrayResult[index]->setLocalZOrder(smallImage->getLocalZOrder());

			pPanel->addChild(_arrayBigLogos[index]);
			pPanel->addChild(_arraySmallLogos[index]);
			pPanel->addChild(_arrayResult[index]);

		}
	}

	void GameResultHistory::ShowProbability()
	{
		if(vtByhistory.size() == 0)
			return;

		std::array<int, 8> LogosNum;

		LogosNum.fill(0);

		for (int i = 0; i < vtByhistory.size(); ++ i)
		{
			const BYTE by = vtByhistory[i];
			++LogosNum[by];
		}

		for (int index = 0; index < AreaCount; ++index)
		{
			char tm[30] = {0};
			sprintf(tm, "%.2f%%", LogosNum[index] * 100.0l /vtByhistory.size());

			_arrayText[index]->setString(tm);
			_arrayText[index]->setVisible(true);
		}
	}

	void GameResultHistory::ShowLastRecord()
	{
		
		_historyIndex = GetMaxIndex();

		ShowRecord();
	}

	void GameResultHistory::CheckRecordIndex()
	{
		_historyIndex = std::max(GetMinIndex(), _historyIndex);

		_historyIndex = std::min(GetMaxIndex(), _historyIndex);
	}

	int GameResultHistory::GetMinIndex()
	{
		return 0;
	}

	int GameResultHistory::GetMaxIndex()
	{
		if(vtByhistory.size() == 0)
		{
			return 0;
		}

		int iMaxIndex = vtByhistory.size() / MAX_COUNT;
		(vtByhistory.size() % MAX_COUNT) == 0 ?iMaxIndex--:iMaxIndex;

		return iMaxIndex;
	}

}