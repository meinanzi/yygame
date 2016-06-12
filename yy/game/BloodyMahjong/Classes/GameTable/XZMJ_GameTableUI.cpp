#include "XZMJ_GameTableUI.h"
#include "GameManager.h"

using namespace std;

namespace XZMJ
{
	/**************************************************************************/

	GameTableUI::GameTableUI()
	{
	}

	GameTableUI::~GameTableUI()
	{
		delete _tableLogic;
		_tableLogic = nullptr;
	}

	GameTableUI* GameTableUI::create(INT deskNo, bool autoCreate)
	{
		auto ui = new GameTableUI;
		if (ui && ui->init(deskNo, autoCreate))
		{
			ui->autorelease();
			return ui;
		}
		CC_SAFE_DELETE(ui);
		return nullptr;
	}

	bool GameTableUI::init(INT deskNo, bool autoCreate)
	{
		if (!HNGameUIBase::init())
		{
			return false;
		}

		//loadResource();
		_mahjongManager = GameManager::create();
		this->addChild(_mahjongManager);

		// logic
		_tableLogic = new GameTableLogic(this, deskNo, autoCreate);
		_tableLogic->sendGameInfo();

		return true;
	}
	

	/******************************************************************/

	/******************************************************************/

	

	void GameTableUI::onGameDisconnect()
	{
		dealLeaveDesk();
	}


	void GameTableUI::dealLeaveDesk()
	{
		if (PlatformLogic()->isConnect())
		{
			GamePlatform::returnPlatform(LayerType::DESKLIST);
		}
		else
		{
		}
	}

	void GameTableUI::agreeGame(const sitDir& dir)
	{
		_mahjongManager->userAgree(dir);
	}

	void GameTableUI::startGame(const bool& autoBegin)
	{
		_mahjongManager->startGame();
	}

	/*****************************************************************************************/


	void GameTableUI::touziAction(const sitDir& startDir ,const Vec2& startPos,  const int& num1, const int& num2, const Vec2& endPos1, const Vec2& endPos2)
	{
		assert(num1 > 0 && num1 < 7);
		assert(num2 > 0 && num2 < 7);
		_touzi0 = num1;
		_touzi1 = num2;
		_startCatchDir = startDir;
		HNAudioEngine::getInstance()->playEffect((MUSIC_PATH + "Sezi.mp3").c_str());
		log("touziA = %d,  touziA.x = %f , touziA.y = %f ", num1, endPos1.x, endPos1.y);
		log("touziB = %d,  touziB.x = %f , touziB.y = %f ", num2, endPos2.x, endPos2.y);

		auto touziA = Sprite::createWithSpriteFrameName(StringUtils::format("touzi%d.png", num1).c_str());
		auto touziB = Sprite::createWithSpriteFrameName(StringUtils::format("touzi%d.png", num2));
		this->addChild(touziA);
		this->addChild(touziB);

		touziA->setVisible(true);
		touziB->setVisible(true);
		touziA->setGlobalZOrder(9999);
		touziB->setGlobalZOrder(9999);
		touziA->setScale(0.5f);
		touziB->setScale(0.5f);
		touziA->setPosition(startPos);
		touziB->setPosition(startPos);

		Vector<SpriteFrame *> vec, vec2;
		vec = getAnimation("touziAction");
		vec2 = getAnimation("touziAction");

		log("vec.size = %d  vec2.size = %d", vec.size(), vec2.size());

		auto animationA = Animation::createWithSpriteFrames(vec);
		animationA->setDelayPerUnit(0.01f);
		animationA->setLoops(25);
		animationA->setRestoreOriginalFrame(true);
		auto animationB = Animation::createWithSpriteFrames(vec2);
		animationB->setDelayPerUnit(0.01f);
		animationB->setLoops(25);
		animationB->setRestoreOriginalFrame(true);
		auto animateA = Animate::create(animationA);
		auto animateB = Animate::create(animationB);

		touziA->runAction(Sequence::create(
			Spawn::create(
			EaseSineOut::create(MoveTo::create(1.0f, endPos1)),
			animateA,
			nullptr),
			DelayTime::create(0.6f),
			CallFunc::create(CC_CALLBACK_0(GameTableUI::releaseSpriteFrameVector, this, vec)), 
			CallFunc::create(CC_CALLBACK_0(Node::removeFromParent, touziA)),
			nullptr));

		touziB->runAction(Sequence::create(
			Spawn::create(
			EaseSineOut::create(MoveTo::create(1.0f, endPos2)),
			animateB,
			nullptr),
			DelayTime::create(0.6f),
			CallFunc::create(CC_CALLBACK_0(GameTableUI::releaseSpriteFrameVector, this, vec2)), 
			CallFunc::create(CC_CALLBACK_0(Node::removeFromParent, touziB)),
			nullptr));

	}
	
	void GameTableUI::startSendBottomCard(std::vector<std::vector<INT>> vvSouthCard)
	{
		_mahjongManager->startSendBottomCard(_startCatchDir, _touzi0, _touzi1, vvSouthCard);
	}

	Vector<SpriteFrame *> GameTableUI::getAnimation(std::string name)
	{
		Vector<SpriteFrame *> vec;
		SpriteFrame * p  = nullptr; 
		for (auto i = 1; ;i++)
		{
			auto fullName = name + StringUtils::format("%d.png", i);
			p = SpriteFrameCache::getInstance()->getSpriteFrameByName(fullName.c_str());
			if (p == nullptr)
			{
				break;
			}
			else
			{
				vec.pushBack(p);
			}
		}
		return vec;
	}

	void GameTableUI::touziCallBack()
	{
		//static int i = 0;
		//++i;
		//if (i == 2)
		//{
		//	startSendBottomCard(
		//}
	}

	void GameTableUI::releaseSpriteFrameVector(cocos2d::Vector<SpriteFrame *>& vec)
	{
		vec.clear();
	}


	void GameTableUI::outCard(const sitDir& dir, const INT& number)
	{
		_mahjongManager->outCard(dir, number);
	}

	void GameTableUI::catchCard(const sitDir& dir, const INT& number, const bool& head)
	{
		_mahjongManager->catchCard(dir, number, head);
	}

	void GameTableUI::setPlayGame(bool bs)
	{
		_mahjongManager->setPlayGame(bs);
	}

	void GameTableUI::reconnected(
			std::vector<std::vector<INT>>& vvHandCard, 
			std::vector<std::vector<INT>>& vvOutCard, 
			std::vector<std::vector<std::vector<INT>>>& vvvActionCard,
			std::vector<std::vector<INT>>& vvMenPai,
			std::vector<bool>& vTrusteeshep,
			std::vector<INT>& vQueMen,
			sitDir& ntDir)
	{
		_mahjongManager->reconnected(vvHandCard, vvOutCard, vvvActionCard, vvMenPai, vTrusteeshep, vQueMen, ntDir);
	}


	/*****************************************************************************************/
	void GameTableUI::addUser(const sitDir& dir, const UserInfoStruct& user)
	{
		_mahjongManager->addUser(dir, user);
	}

	void GameTableUI::removeUser(const sitDir& dir)
	{
		_mahjongManager->userLeave(dir);
	}

	/******************************************************************/
};
