#include "DriftingCarGameBoard.h"
#include "DriftingCarGameCell.h"

#define BOARD_SIZE Size(798, 525)


namespace DriftingCar
{
	float GameBoard::speed_level1 = 0.2f;
	float GameBoard::speed_level2 = 0.06f;//0.4
	float GameBoard::speed_level3 = 0.3f;

	GameBoard* GameBoard::create(Node* pNode)
	{
		GameBoard* board = new GameBoard();
		if(board->init(pNode))
		{
			board->ignoreAnchorPointForPosition(false);
			board->autorelease();
			return board;
		}
		CC_SAFE_DELETE(board);
		return nullptr;
	}

	bool GameBoard::init(Node* pNode)
	{
		if(!HN::HNLayer::init())
		{
			return false;
		}

		this->ignoreAnchorPointForPosition(false);
		loadCells(pNode);

		return true;
	}

	/*
	 * 结束前两秒开奖。
	 * 三种速度，开始较慢，中途加速，开奖的时候减速
	 */

	void GameBoard::StartPlayOther(int endIndex, int btime)
	{
		_startIndex   = 31;//31下标开始
		_endIndex     = endIndex - 1;
		_currentIndex = 31; 

		_totalTime    = btime;

		SetCarStartPosition();

		showAction(_startIndex,speed_level1);

		_cells[_startIndex]->turnOn();

		//为每个阶段分配时间
		_levelTime[2] = CAR_LOGO_COUNT * speed_level3;
		float perTime = _totalTime - _levelTime[2];
		_levelTime[0] = 0.1 * perTime;
		_levelTime[1] = 0.9 * perTime;

		_runTime = 0.f;

		schedule(schedule_selector(GameBoard::switchCell), speed_level1);

	}

	void GameBoard::startPlay( int endIndex, BYTE btime)
	{
		_startIndex   = 31;//31下标开始

		_currentIndex = 31; 

		_totalTime    = btime;
		
		SetCarStartPosition();

		showAction(_startIndex,speed_level1);

		_cells[_startIndex]->turnOn();

		_jiashudu = 0.5f;
		_iSumCount = CAR_LOGO_COUNT * 3 + endIndex;
		_iRoundcount = 0;

		schedule(schedule_selector(GameBoard::CarPlay), _jiashudu);

	}

	GameBoard::GameBoard()
		: _startIndex(0)
		, _endIndex(0)
		, _currentIndex(0)
		, _level1Count(0)
		, _level2Count(0)
		, _level3Count(0)
		, _totalTime(0)
		, _runTime(0)
		,CallBackPlayAnimation(nullptr)
		,CallBackCarIndex(nullptr)
	{
		
	}

	GameBoard::~GameBoard()
	{

	}

	void GameBoard::loadCells(Node* layout)
	{
		
		for(int i = 0; i < CAR_LOGO_COUNT; i++)
		{
			char strName[20] = {0};
			sprintf(strName, "Sprite_%d", i + 1);

			Sprite* pSprite = (Sprite*) layout->getChildByName(strName);

			_Vec2Car[i] = pSprite->getPosition();

			GameCell* cell = GameCell::create();
			
			cell->setTexture(pSprite->getTexture());
			cell->setTextureRect(pSprite->getTextureRect());
			cell->setAnchorPoint(Vec2(0.5,0.5));
			cell->setPosition(pSprite->getPosition());
			cell->setLocalZOrder(pSprite->getLocalZOrder());
			
			cell->setOpacity(0);

			pSprite->setVisible(false);

			this->addChild(cell);

			_cells.push_back(cell);
		}

		//不能改变初始化顺序
		InitCarCoord();

		InitCarAnimation();

	}
	void GameBoard::CarPlay(float delay)
	{
		_cells[_currentIndex]->turnOff();
		HideAction(_currentIndex);

		_currentIndex = (_currentIndex + 1) % CAR_LOGO_COUNT;
		_iRoundcount++;
		CallBackCarIndex(_currentIndex);//执行回调函数

		_cells[_currentIndex]->turnOn();
		showAction(_currentIndex, delay);

		CCLOG("time = %f",_runTime+=delay);
		CCLOG("-----------------%d",_iRoundcount);

		if(_iRoundcount <= 6)
		{
			unschedule(schedule_selector(GameBoard::CarPlay));
			_jiashudu = std::max(0.03,_jiashudu - 0.04);
			schedule(schedule_selector(GameBoard::CarPlay), _jiashudu);

		}
		else if(_iRoundcount < _iSumCount / 3 )
		{
			unschedule(schedule_selector(GameBoard::CarPlay));
			_jiashudu =0.06;
			schedule(schedule_selector(GameBoard::CarPlay), _jiashudu);
		}

		else if(_iRoundcount >= _iSumCount /3 && _iRoundcount < _iSumCount/3*2)
		{
			unschedule(schedule_selector(GameBoard::CarPlay));
			_jiashudu =0.03;
			schedule(schedule_selector(GameBoard::CarPlay), _jiashudu);
		}
		else if(_iRoundcount >= _iSumCount/3*2 && _iRoundcount < _iSumCount - 6)
		{
			unschedule(schedule_selector(GameBoard::CarPlay));
			_jiashudu = 0.06;
			schedule(schedule_selector(GameBoard::CarPlay), _jiashudu);

		}
		else
		{
			unschedule(schedule_selector(GameBoard::CarPlay));
			if(_iRoundcount == _iSumCount)
			{
				CallBackPlayAnimation(this);
				return ;
			}
			_jiashudu = _jiashudu + 0.06;
			schedule(schedule_selector(GameBoard::CarPlay), _jiashudu);
		}

	}
	void GameBoard::switchCell(float delta)
	{
		_cells[_currentIndex]->turnOff();
		HideAction(_currentIndex);

		_currentIndex = (_currentIndex + 1) % CAR_LOGO_COUNT;

		CallBackCarIndex(_currentIndex);//执行回调函数

		_cells[_currentIndex]->turnOn();
		showAction(_currentIndex, delta);

		_runTime += delta;

		do 
		{
			if(_runTime > _levelTime[0] + _levelTime[1])
			{
				unschedule(schedule_selector(GameBoard::switchCell));
				int repeat = (_endIndex - _currentIndex - 1 + CAR_LOGO_COUNT) % CAR_LOGO_COUNT;
				schedule(schedule_selector(GameBoard::switchCell), speed_level3, repeat, 0);

				if(repeat == 0)
				{
					CallBackPlayAnimation(this);
				}
				
				break;
			}
			else if(_runTime > _levelTime[0])
			{
				unschedule(schedule_selector(GameBoard::switchCell));
				schedule(schedule_selector(GameBoard::switchCell), speed_level2);
				break;
			}
			
		} while (0);
	}

	void GameBoard::InitCarCoord()
	{

		for (int i = 1; i <=7; i++)
		{
			_Vec2Car[i].y -= 62;
		}
		int iCount = 17+7;

		for (int i = 17; i < iCount; ++i)
		{
			_Vec2Car[i].y += 62;
		}

		//左半球坐标
		const int iStepLeft = 8;

		_Vec2Car[iStepLeft].x -= 19;
		_Vec2Car[iStepLeft].y -= 61;

		_Vec2Car[iStepLeft+1].x -= 44;
		_Vec2Car[iStepLeft+1].y -= 48;

		_Vec2Car[iStepLeft+2].x -= 56 ;
		_Vec2Car[iStepLeft+2].y -= 34;

		_Vec2Car[iStepLeft+3].x -= 60;
		_Vec2Car[iStepLeft+3].y -= 18;

		_Vec2Car[iStepLeft+4].x -= 62;
		_Vec2Car[iStepLeft+4].y -= 0;

		_Vec2Car[iStepLeft+5].x -= 60;
		_Vec2Car[iStepLeft+5].y += 18;

		_Vec2Car[iStepLeft+6].x -= 56;
		_Vec2Car[iStepLeft+6].y += 34;

		_Vec2Car[iStepLeft+7].x -= 44;
		_Vec2Car[iStepLeft+7].y += 48;

		_Vec2Car[iStepLeft+8].x -= 19;
		_Vec2Car[iStepLeft+8].y += 61;

		const int iStepRight = 24;

		//右半球
		_Vec2Car[iStepRight].x += 19;
		_Vec2Car[iStepRight].y += 61;

		_Vec2Car[iStepRight+1].x += 44;
		_Vec2Car[iStepRight+1].y += 48;

		_Vec2Car[iStepRight+2].x += 56 ;
		_Vec2Car[iStepRight+2].y += 34;

		_Vec2Car[iStepRight+3].x += 60;
		_Vec2Car[iStepRight+3].y += 18;

		_Vec2Car[iStepRight+4].x += 62;
		_Vec2Car[iStepRight+4].y -= 0;

		_Vec2Car[iStepRight+5].x += 60;
		_Vec2Car[iStepRight+5].y -= 18;

		_Vec2Car[iStepRight+6].x += 56;
		_Vec2Car[iStepRight+6].y -= 34;

		_Vec2Car[iStepRight+7].x += 44;
		_Vec2Car[iStepRight+7].y -= 48;

		//第1个
		_Vec2Car[0].x += 19;
		_Vec2Car[0].y -= 61;
	}

	void GameBoard::SetCarStartPosition()
	{
		_spriteCar->setPosition(_Vec2Car[30]);
		_spriteCar->setRotation(-18 * 2);
	}

	void GameBoard::InitCarAnimation()
	{

		_spriteCar = Sprite::create("DriftingCar/CardUi/Res/carAnimate/1.png");

		const auto& p = _cells[0];
		_spriteCar->setLocalZOrder(p->getLocalZOrder() + 5);
		_spriteCar->setOpacity(0);

		p->getParent()->addChild(_spriteCar);

		for (int i = 0; i < 32; i++)
		{
			auto tt = CCMoveTo::create(0.1f,_Vec2Car[i]);

			int iRotate = 0;
			if(i < 8)
			{
				iRotate = 0;
			}
			else if(i >=8 && i <= 17)
			{
				iRotate = (i-7)*18;

			}else if(i >17 && i <=23)
			{
				iRotate = 180;
			}
			else
			{
				iRotate = 180 + (i-23)*18;
			}

			auto rotate  = CCRotateTo::create(0.1,iRotate);


			_vtAction.pushBack(CCSpawn::create(tt, rotate,nullptr));

		}

	}

	void GameBoard::HideAction( int index )
	{
		auto localAction = _vtAction.at(index);
		localAction->stop();
		_spriteCar->stopAction(localAction);
	}

	void GameBoard::showAction( int index ,float fduration)
	{
		auto localAction = (Spawn*)_vtAction.at(index);
		localAction->setDuration(fduration);

		_spriteCar->runAction(localAction);

	}

	void GameBoard::HideCarAndAllCell()
	{

		_spriteCar->runAction(FadeOut::create(1.0f));

		for (int i = 0; i < _cells.size(); i++)
		{
			_cells[i]->stopAllActions();
			_cells[i]->runAction(FadeOut::create(1.0f));
		}
		
	}

	void GameBoard::showCarStart()
	{
		_spriteCar->setRotation(-36);				//摆正位置
		_spriteCar->setPosition(_Vec2Car[30]);		//起点
		_spriteCar->runAction(FadeIn::create(3.0f));
	}

	void GameBoard::ShowCarBlinkLogos( int index )
	{
		int itype = index % CAR_LOGO_TYPE;

		for (int i = 0; i < 4; i++)
		{
			int iLogos = (i * CAR_LOGO_TYPE) + itype;

			_cells[iLogos]->setOpacity(255);
			_cells[iLogos]->runAction(Sequence::create(Blink::create(3,9), Blink::create(3,16), nullptr));
		}
	}

	

}