#include "DriftingCarGameCell.h"

namespace DriftingCar
{
	GameCell* GameCell::create()
	{
		GameCell* cell = new GameCell();
		if(cell->init())
		{
			cell->ignoreAnchorPointForPosition(false);
			cell->autorelease();
			return cell;
		}
		CC_SAFE_DELETE(cell);
		return cell;
	}

	void GameCell::turnOn()
	{
		this->setVisible(true);
		this->setOpacity(255);
	}

	void GameCell::turnOff()
	{
		runAction(FadeOut::create(0.5f));
	}

	bool GameCell::init()
	{
		if(!HN::HNSprite::init())
		{
			return false;
		}
		return true;
	}

	GameCell::GameCell()
	{

	}

	GameCell::~GameCell()
	{

	}

}
