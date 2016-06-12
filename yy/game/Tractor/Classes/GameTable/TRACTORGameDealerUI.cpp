#include "TRACTORGameDealerUI.h"
#include "TRACTORGameChipUI.h"
#include "TRACTORPokerCard.h"
#include "TRACTORGameTableUI.h"
#include "HNUIExport.h"
#include "TRACTORDrawEllipse.h"

USING_NS_CC;
using namespace HN;
namespace TRACTOR
{
	static const  char*   CHIP_FILENAME  = "tractor/table/texas_chip05.png";

	static const  char*  GAME_BIPAI_FALSE		=			"tractor/sound/bipai_shu.mp3";
	static const  char*  GAME_BIPAI_WIN			=			"tractor/sound/bipai_yin.mp3";
	static const  char*  GAME_CLICK				=			"tractor/sound/click.mp3";
	static const  char*  GAME_FAPAI				=			"tractor/sound/fapai.mp3";
	static const  char*  GAME_ADD				=			"tractor/sound/jiazhu.mp3";
	static const  char*  GAME_SHOUCHOUMA		=			"tractor/sound/shouchouma.mp3";
	static const  char*  GAME_TEJIANGZHONG		=			"tractor/sound/tejiangzhong.mp3";
	static const  char*  GAME_XIAZHU			=			"tractor/sound/xiazhu.mp3";

	enum 
	{
		eZCommunityCard = 10,//公共牌
	};

	enum
	{
		eDealerNode,
		eDealerStart,
		eDealerFlop = 10,
		eDealerTurn ,
		eDealerRiver,
		eDealerEnd,
	};

	GameDelearUI* GameDelearUI::create()
	{
		GameDelearUI* dealer = new GameDelearUI();
		if(dealer->init())
		{
			dealer->autorelease();
			return dealer;
		}
		else
		{
			CC_SAFE_DELETE(dealer);
			return nullptr;
		}
	}

	void GameDelearUI::betMoney(BYTE seatNo, LLONG money)
	{
		if(!seatNoOk(seatNo))
		{
			return;
		}

		GameChipUI* chip = GameChipUI::create(CHIP_FILENAME);
		chip->setValue(money);
		chip->setScale(0.5f);
		chip->setTag(seatNo);
		chip->setPosition(getSeatPoint(seatNo));

		_chipsPool->addChild(chip);

		Vec2 chipPoint;
		bool flag;
		do{
			chipPoint = _ellipse->getEllipseOrigin();
			auto xOffset = fmod(rand()*(rand() % 2 ? -1 : 1), _ellipse->getEllipseX());
			auto yOffset = fmod(rand()*(rand() % 2 ? -1 : 1), _ellipse->getEllipseY());

			chipPoint.x += xOffset;
			chipPoint.y += yOffset;

			flag = _ellipse->isInEllipse(chipPoint);
		} while (!flag);

		chip->runAction(MoveTo::create(0.1f, chipPoint));
		HNAudioEngine::getInstance()->playEffect(GAME_XIAZHU);

		//chip->runAction(Sequence::create(
		//	EaseIn::create(MoveTo::create(0.3f,getTablePoint(seatNo)),2.5f),
		//	CallFuncN::create(CC_CALLBACK_1(GameDelearUI::checkBet,this)),
		//	nullptr));
	}

	//移动庄家
	void GameDelearUI::dealButton(BYTE buttonStation)
	{
		if(!seatNoOk(buttonStation))
		{
			return;
		}

		_buttonStation = buttonStation;
		Vec2 vec = getButtonPoint();
		if(vec == Vec2::ZERO)
		{
			return;
		}
		_dealer->setPosition(_dealerPoint);
		if(!_dealer->isVisible())
		{
			_dealer->setVisible(true);
		}
		_dealer->runAction(EaseIn::create(MoveTo::create(0.3f,vec), 2.5f));
	}

	//给玩家发手牌
	void GameDelearUI::dealHandCard(const std::vector<THandCard>& handCard)
	{
		_handCard = handCard;

		// counter for the cards dealed
		_curCount = 0;

		//schedule(schedule_selector(GameDelearUI::dispatchHandCard), 0.2f, 3 * handCard.size() -1, 0);
		dispatchHandCard();
	}

	//void GameDelearUI::dispatchHandCard(float delta)
	void GameDelearUI::dispatchHandCard()
	{
		////////////////////////////////////////////////////////////////
		// shuffle cards

		// radius
		float radius = 250;

		// Total angle
		float alpha = 40;
		float y_adjustment = 230;

		float beta = (180 - alpha) / 2;

		// angle value TO radians value
		auto r_alpha = alpha * M_PI / 180;
		auto r_beta = beta * M_PI / 180;
		auto r_alphaSplit = r_alpha / (TOTAL_CARDS - 1);

		int delaycounter = 0;
		auto dealerPosition = _dealerPoint;

		for (int i = 0; i < TOTAL_CARDS; i++, delaycounter++)
		{
			auto x_offset = radius*cos(r_alphaSplit*i + r_beta);

			auto y_offset = (-1)*radius*sin(r_alphaSplit*i + r_beta) + y_adjustment;
			auto rotateAngle = (r_alphaSplit*i + r_beta) * 180 / M_PI - 90;

			_cardSuite[i]->setPosition(Vec2(dealerPosition.x + x_offset, dealerPosition.y + y_offset));
			_cardSuite[i]->runAction(Sequence::create(
				RotateBy::create(0.01f, rotateAngle),
				DelayTime::create(0.01f*delaycounter),
				CallFunc::create([=](){
					_cardSuite[i]->setVisible(true);
			})
				, nullptr));
		}

		////////////////////////////////////////////////////////////////
		// dear cards

		schedule(schedule_selector(GameDelearUI::showUserHandCard), 0.3f, 3 * _handCard.size() - 1, 0);

		/*	
		HNAudioEngine::getInstance()->playEffect(GAME_FAPAI);
		_curCount++;
		int index = _curCount;
		if(_curCount >= _handCard.size())
		{
		index = _curCount % _handCard.size();
		}

		PokerCard* card = PokerCard::create(0x00);
		this->addChild(card);
		card->setScale(0.2f);
		card->setPosition(_dealerPoint);
		card->setCardValue(0);
		//card->setCardFrameName(card->getCardFileName());
		int* cdata = new int(_curCount);
		card->setUserData(cdata);
		card->runAction(Sequence::create(Spawn::create(EaseIn::create(MoveTo::create(0.3f, getSeatPoint(_handCard[index].bySeatNo)), 2.5f),
		ScaleTo::create(0.3f, 0.5f),nullptr), 
		RemoveSelf::create(true),
		CallFuncN::create(CC_CALLBACK_1(GameDelearUI::showUserHandCard,this)),
		nullptr));
		*/	
	}

	//void GameDelearUI::showUserHandCard(Node* pSender)
	void GameDelearUI::showUserHandCard(float delta)
	{
		int playercount = _handCard.size();

		_curCount++;
		int cardIndex = (_curCount - 1) / playercount;
		int playerIndex = (_curCount - 1) % playercount;

		int theTopIndexOfCardSuite = TOTAL_CARDS - _curCount;

		GameTableUI* table = (GameTableUI*)this->getParent();
		auto player = table->getPlayer(_handCard[playerIndex].bySeatNo);
		//auto playerposition = player->getPosition();

		auto move = MoveTo::create(0.5f, getSeatPoint(_handCard[playerIndex].bySeatNo));
		auto ease = EaseSineOut::create(move);
		auto rotate = RotateTo::create(0.5f, 180);
		auto spwn = Spawn::create(ease, rotate, nullptr);

		_cardSuite[theTopIndexOfCardSuite]->runAction(Sequence::create(
			spwn,
			CallFunc::create([=](){
				HNAudioEngine::getInstance()->playEffect(GAME_FAPAI);
				_cardSuite[theTopIndexOfCardSuite]->setVisible(false);
				player->setHandCard(cardIndex, 0x00);
		}),
			nullptr));

		if (_curCount >= _handCard.size() * 3)
		{
			schedule(schedule_selector(GameDelearUI::recycleCardSuite), 0.5f, 0, 0.6f);
		}

		/*
		int cardIndex = 0;
		unsigned int index = *(int*)pSender->getUserData();
		delete pSender->getUserData();
		if(index >= _handCard.size())
		{
		cardIndex = index / _handCard.size();
		index = index % _handCard.size();
		}
		if(_handCard[index].bySeatNo != INVALID_DESKNO)
		{
		GameTableUI* table = (GameTableUI*)this->getParent();
		auto player = table->getPlayer(_handCard[index].bySeatNo);
		if(player)
		{
		player->setHandCard(cardIndex, _handCard[index].byCards[cardIndex]);
		}
		}
		*/
	}

	// recycle card suite
	void GameDelearUI::recycleCardSuite(float delta)
	{
		auto delaycounter = 0;
		for (int i = TOTAL_CARDS - 1 - _curCount; i >= 0; i--)
		{
			if (_cardSuite[i]->isVisible() == true)
			{
				_cardSuite[i]->runAction(Sequence::create(
					DelayTime::create(0.005f*delaycounter++),
					CallFunc::create([=](){
						_cardSuite[i]->setVisible(false);
						_cardSuite[i]->setRotation(0);
				}),
					nullptr));
			}
			else
			{
				_cardSuite[i]->setRotation(0);
			}

		}
	}

	void GameDelearUI::recycleChips(BYTE winnerSeatNo)
	{
		auto winnerpos = getSeatPoint(winnerSeatNo);
		for (auto chip : _chipsPool->getChildren())
		{
			chip->runAction(Sequence::create(
				MoveTo::create(0.5f, winnerpos),
				CallFunc::create([=](){
					chip->removeFromParent();
			}),
				nullptr));
		}
		return;
	}

	//发翻牌
	void GameDelearUI::dealFlopCard(const std::vector<BYTE>& flopCard)
	{
		if(flopCard.size() != 3)
		{
			return;
		}

		for(unsigned int i = 0; i < flopCard.size(); i++)
		{
			HNAudioEngine::getInstance()->playEffect(GAME_FAPAI);
			PokerCard* pCard = (PokerCard*)_communityCard[i];
			pCard->setCardValue(flopCard[i]);
			//pCard->setCardFrameName(pCard->getCardFileName());
			pCard->setPosition(_communityCardPoints[0]);
			pCard->setVisible(false);
		}

		dispatchCommunityCard(eDealerFlop);
	}

	void GameDelearUI::moveFlopCard()
	{
		for(unsigned int i = 1; i < 3; i++)
		{
			_communityCard[i]->runAction(MoveTo::create(0.1 * i, _communityCardPoints[i]));
		}
	}

	//发转牌
	void GameDelearUI::dealTurnCard(BYTE turnCard)
	{
		HNAudioEngine::getInstance()->playEffect(GAME_FAPAI);
		PokerCard* card = (PokerCard*)_communityCard[3];
		card->setCardValue(turnCard);
		//card->setCardFrameName(card->getCardFileName());
		card->setPosition(_communityCardPoints[3]);
		card->setVisible(false);

		dispatchCommunityCard(eDealerTurn);
	}

	//发河牌
	void GameDelearUI::dealRiverCard(BYTE riverCard)
	{
		HNAudioEngine::getInstance()->playEffect(GAME_FAPAI);
		PokerCard* card = (PokerCard*)_communityCard[4];
		card->setCardValue(riverCard);
		//card->setCardFrameName(card->getCardFileName());
		card->setPosition(_communityCardPoints[4]);
		card->setVisible(false);

		dispatchCommunityCard(eDealerRiver);
	}

	void GameDelearUI::dispatchCommunityCard(int type)
	{
		Action* action = nullptr;
		if(type == eDealerFlop)
		{
			action = getFlopAction();
		}
		else if(type == eDealerTurn)
		{
			action = getTurnAction();
		}
		else if(type == eDealerRiver)
		{
			action = getRiverAction();
		}
		else
		{

		}

		if(action == nullptr)
		{
			return;
		}

		PokerCard* tmpCard = PokerCard::create(0x00);
		tmpCard->setPosition(_dealerPoint);
		//tmpCard->setScale(0.4f);
		tmpCard->setCardValue(0);
		//tmpCard->setCardFrameName(tmpCard->getCardFileName());
		tmpCard->setTag(type);
		this->addChild(tmpCard,eZCommunityCard);
		tmpCard->runAction(action);
	}

	void GameDelearUI::openCommunityCard(cocos2d::Node* pNode)
	{
		int tag = pNode->getTag();
		if(tag == eDealerFlop)
		{
			_communityCard[0]->setVisible(true);
			_communityCard[1]->setVisible(true);
			_communityCard[2]->setVisible(true);
		}
		else if(tag == eDealerTurn)
		{
			_communityCard[3]->setVisible(true);
		}
		else if(tag == eDealerRiver)
		{
			_communityCard[4]->setVisible(true);
		}
	}

	cocos2d::Action* GameDelearUI::getFlopAction()
	{
		Action* action = nullptr;
		auto moveTo = MoveTo::create(0.5f, _communityCardPoints[0]);
		auto scale = ScaleTo::create(0.5f, 0.8f);
		auto openCard = CallFuncN::create(CC_CALLBACK_1(GameDelearUI::openCommunityCard,this));
		auto moveFlop = CallFunc::create(CC_CALLBACK_0(GameDelearUI::moveFlopCard,this));
		action = Sequence::create(Spawn::create(moveTo,scale, nullptr),
			RemoveSelf::create(true),
			openCard,
			moveFlop,
			nullptr);
		return action;
	}

	cocos2d::Action* GameDelearUI::getTurnAction()
	{
		Action* action = nullptr;
		auto moveTo = MoveTo::create(0.5f, _communityCardPoints[3]);
		auto scale = ScaleTo::create(0.5f, 0.8f);
		auto openCard = CallFuncN::create(CC_CALLBACK_1(GameDelearUI::openCommunityCard,this));
		action = Sequence::create(Spawn::create(moveTo,scale, nullptr),
			RemoveSelf::create(true),
			openCard,
			nullptr);
		return action;
	}

	cocos2d::Action* GameDelearUI::getRiverAction()
	{
		Action* action = nullptr;
		auto moveTo = MoveTo::create(0.5f, _communityCardPoints[4]);
		auto scale = ScaleTo::create(0.5f, 0.8f);
		auto openCard = CallFuncN::create(CC_CALLBACK_1(GameDelearUI::openCommunityCard,this));
		action = Sequence::create(Spawn::create(moveTo,scale, nullptr),
			RemoveSelf::create(true),
			openCard,
			nullptr);
		return action;
	}

	//收牌
	void GameDelearUI::dealCollectCard()
	{

	}

	//给玩家派奖（先收筹码，再派奖）
	void GameDelearUI::dealWinPool(const std::vector<std::vector<LLONG>> &winPool)
	{
		_winPool = winPool;
		HNAudioEngine::getInstance()->playEffect(GAME_SHOUCHOUMA);

		this->runAction(Sequence::create(
			CallFunc::create(CC_CALLBACK_0(GameDelearUI::collectChip ,this)),
			DelayTime::create(0.6f),
			CallFunc::create(CC_CALLBACK_0(GameDelearUI::sendPrize,this)),
			nullptr));
	}

	bool GameDelearUI::init()
	{
		if(!HNLayer::init())
		{
			return false;
		}
		_tableChip.resize(PLAY_COUNT);

		//添加5张公共牌，以后一直缓存使用
		_communityCardPoints.resize(5);
		_communityCard.resize(5);
		for(int i = 0; i < 5; i++)
		{
			PokerCard* card = PokerCard::create(0x00);
			this->addChild(card,eZCommunityCard + i);
			card->setVisible(false);
			//card->setScale(0.8f);
			_communityCard[i] = card;
		}


		//庄家
		_dealer = Sprite::create("tractor/table/dealer.png");
		this->addChild(_dealer,50);
		_dealer->setVisible(false);
		_dealer->setPosition(Vec2::ZERO);

		// init card suite with back image, total 52 cards in a suite
		for (int i = 0; i < TOTAL_CARDS; i++)
		{
			_cardSuite[i] = PokerCard::create(0x00);
			this->addChild(_cardSuite[i], i);
			_cardSuite[i]->setScale(0.8f);
			_cardSuite[i]->setVisible(false);
		}

		// init the ellipse area chip pool
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		auto e_origin = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
		_ellipse = new DrawEllipse(e_origin, 150, 50);

		// init the chip pool
		_chipsPool = Layer::create();
		_chipsPool->setPosition(origin);
		this->addChild(_chipsPool);

		return true;
	}

	GameDelearUI::GameDelearUI():_buttonStation(INVALID_DESKNO)
	{


	}


	GameDelearUI::~GameDelearUI()
	{

	}

	Vec2 GameDelearUI::getButtonPoint()
	{
		if(_buttonStation != INVALID_DESKNO)
		{
			return _tablePoints[_buttonStation] + Vec2(-54 / 2.0f,0);
		}
		else
		{
			return Vec2::ZERO;
		}
	}

	cocos2d::Vec2 GameDelearUI::getSeatPoint(BYTE seatNo)
	{
		return (seatNoOk(seatNo)) ?_seatPoints[seatNo]: Vec2::ZERO;
	}

	cocos2d::Vec2 GameDelearUI::getTablePoint(BYTE seatNo)
	{
		return (seatNoOk(seatNo))? _tablePoints[seatNo]: Vec2::ZERO;
	}

	void GameDelearUI::collectChip()
	{
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			if(_tableChip[i] != nullptr)
			{
				_tableChip[i]->runAction(Sequence::create(
					EaseOut::create(MoveTo::create(0.4f,_dealerPoint), 2.5f),
					RemoveSelf::create(true),
					nullptr));
				_tableChip[i] = nullptr;
			}
		}
	}

	void GameDelearUI::sendPrize()
	{
		//for(int poolIndex = 0; poolIndex < PLAY_COUNT; poolIndex++)
		//{
		//	for(int seatNo = 0; seatNo < PLAY_COUNT; seatNo++)
		//	{
		//		//按照奖池来分配
		//		if(_winPool[seatNo][poolIndex] > 0)
		//		{
		//			auto chip = GameChipUI::create(CHIP_FILENAME);
		//			chip->setValue(_winPool[seatNo][poolIndex]);
		//			chip->setAnchorPoint(Vec2(0.5f, 0.5f));
		//			chip->setPosition(_dealerPoint);
		//			chip->setScale(0.5f);
		//			this->addChild(chip);
		//			chip->runAction(Sequence::create(
		//				EaseIn::create(MoveTo::create(0.6f, getSeatPoint(seatNo)), 2.5f),
		//				RemoveSelf::create(true),
		//				nullptr));
		//		}
		//	}
		//}
	}

	//void GameDelearUI::checkBet(cocos2d::Node* pNode)
	//{
	//	GameChipUI* pChip = (GameChipUI*)pNode;
	//
	//	BYTE seatNo = pChip->getTag();
	//	if(!seatNoOk(seatNo))
	//	{
	//		return;
	//	}
	//
	//	//桌上没有筹码是保留，否则移除掉下的筹码并更新桌上的筹码
	//	if(_tableChip[seatNo] == nullptr)
	//	{
	//		_tableChip[seatNo] = pChip;
	//	}
	//	else
	//	{
	//		_tableChip[seatNo]->setValue(pChip->getValue());
	//		pChip->removeFromParentAndCleanup(true);
	//	}
	//}

	void GameDelearUI::setDealerPoint(cocos2d::Vec2 point)
	{
		_dealerPoint = point;
		if(_dealer != nullptr)
		{
			_dealer->setPosition(point);
		}
	}

	void GameDelearUI::setTablePoint(std::vector<cocos2d::Vec2>& points)
	{
		_tablePoints = points;
	}

	void GameDelearUI::setSeatPoint(std::vector<cocos2d::Vec2>& points)
	{
		_seatPoints = points;
	}

	void GameDelearUI::setCommunityPoint(const std::vector<cocos2d::Vec2>& points)
	{
		if(points.size() != 5)
		{
			return;
		}
		_communityCardPoints = points;
		for(unsigned int i = 0; i < points.size(); i++)
		{
			_communityCard[i]->setPosition(points[i]);
		}
	}

	//void GameDelearUI::rotateSeat(int offset)
	//{
	//	//庄家位置
	//	_buttonStation = (_buttonStation + offset + PLAY_COUNT) % PLAY_COUNT;
	//	_dealer->setPosition(getButtonPoint());
	//
	//	//桌上筹码位置或手牌位置
	//	std::vector<GameChipUI*> newChip;
	//	newChip.resize(PLAY_COUNT);
	//
	//	std::vector<THandCard> newHandCard;
	//	newHandCard.resize(PLAY_COUNT);
	//
	//	for(int i = 0; i < PLAY_COUNT; i++)
	//	{
	//		int index = (i + offset + PLAY_COUNT) % PLAY_COUNT;
	//		if(_tableChip[i] != nullptr)
	//		{
	//			_tableChip[i]->setPosition(getTablePoint(index));
	//			newChip[index] = _tableChip[i];
	//		}
	//		newHandCard[index] = _handCard[i];
	//	}
	//
	//	_handCard = newHandCard;
	//	_tableChip = newChip;
	//}

	void GameDelearUI::setButtonStation(BYTE buttonStation)
	{
		_buttonStation = buttonStation;
	}

	bool GameDelearUI::seatNoOk(BYTE seatNo)
	{
		return (seatNo >= 0 && seatNo < PLAY_COUNT);
	}

	void GameDelearUI::clear()
	{
		for(int i = 0; i < 5; i++)
		{
			_communityCard[i]->setVisible(false);
		}
		_dealer->setVisible(false);
	}

	void GameDelearUI::dealFold(BYTE seatNo)
	{
		PokerCard* card = PokerCard::create(0x00);
		card->setCardValue(0);
		//card->setCardFrameName(card->getCardFileName());
		addChild(card);
		//card->setScale(0.8f);
		card->setPosition(getSeatPoint(seatNo));

		card->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.2f, 0.2f), EaseOut::create(MoveTo::create(0.2f, _dealerPoint), 2.5f), nullptr), 
			RemoveSelf::create(true),
			nullptr));
	}
}