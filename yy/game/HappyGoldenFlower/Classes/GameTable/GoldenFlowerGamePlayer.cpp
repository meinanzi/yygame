#include "GoldenFlowerGamePlayer.h"
#include "GoldenFlowerPokerCard.h"
#include "GoldenFlowerGameTableUI.h"
#include "GoldenFlowerGameUserMessageBox.h"
#include "GoldenFlowerMessageHead.h"

#include "HNCommon/HNConverCode.h"
#include "HNLobbyExport.h"

/************************line*********************************/
namespace goldenflower
{
	#define WOMEN_HEAD      "goldenflower/table/women_head%d.png"
	#define MEN_HEAD        "goldenflower/table/men_head%d.png"
	#define BG_SRC          "goldenflower/table/playerBG.png"
	#define PROGRESS_SRC    "goldenflower/table/countdown.png"
	#define CHIP_BACKGROUND "goldenflower/table/gamingRoomChipBG.png"

	PlayerUI* PlayerUI::create(LLONG userId, BYTE seatNo)
	{
		PlayerUI* player = new PlayerUI(userId, seatNo);
		if(player && player->init())
		{
			player->autorelease();
			return player;
		}
		else
		{
			CC_SAFE_DELETE(player);
			return nullptr;
		}
	}

	PlayerUI::PlayerUI(LLONG userId, BYTE seatNo)
		: _chip(0)
		, _headSp(nullptr)
		, _nameText(nullptr)
		, _chipText(nullptr)
		, _bg(nullptr)
		, _progressTimer(nullptr)
		, _leftTime(0)
		, _userId(userId)
		, _seatNo(seatNo)
		, _isPlaying(false)
		, _isQuit(false)
	{
		memset(_handCard, 0, sizeof(_handCard));
	}

	PlayerUI::~PlayerUI()
	{
		unschedule(schedule_selector(PlayerUI::onWaitTimer));
	}

	bool PlayerUI::init()
	{
		if(!HN::HNLayer::init())
		{
			return false;
		}

		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(PlayerUI::onTouchBegan, this);
		listener->onTouchEnded = CC_CALLBACK_2(PlayerUI::onTouchEnded, this);
		listener->setSwallowTouches(true);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		_bg = HNSprite::create(BG_SRC);
		this->addChild(_bg,1);
		this->setAnchorPoint(Vec2(0.5, 0.5));

		Size size = _bg->getContentSize();
		this->setContentSize(size);
		this->ignoreAnchorPointForPosition(false);

		auto timerSp = Sprite::create(PROGRESS_SRC);
		_progressTimer = ProgressTimer::create(timerSp);
		this->addChild(_progressTimer, 3);
		_progressTimer->setScaleX(size.width / timerSp->getContentSize().width);
		_progressTimer->setScaleY(size.height / timerSp->getContentSize().height);
		_progressTimer->setPosition(size.width / 2, size.height / 2);
		_progressTimer->setPercentage(0);
		_progressTimer->setVisible(false);
		_progressTimer->setReverseProgress(true);

		_bg->setAnchorPoint(Vec2::ZERO);
		_bg->setPosition(Vec2::ZERO);

		_nameText = Text::create("", "Arial", 18);
		_nameText->setAnchorPoint(Vec2(0.5f, 0.5f));
		_nameText->setPosition(Vec2(size.width*0.5, size.height*0.9));
		_nameText->setColor(Color3B(144,238,144));
		this->addChild(_nameText, 3);

		_chipText = Text::create("", "Arial", 20);
		_chipText->setAnchorPoint(Vec2(0.f, 0.f));
		_chipText->setPosition(Vec2(size.width*0.20, size.height*0.02));
		_chipText->setColor(Color3B(255, 255, 0));
		this->addChild(_chipText, 3);

		_chipBackground = HNSprite::create(CHIP_BACKGROUND);
		_chipBackground->setAnchorPoint(Vec2(0.f, 0.f));
		_chipBackground->setPosition(Vec2(0, size.height*0.15));
		_chipBackground->setScaleX(size.width / _chipBackground->getContentSize().width);
		_chipBackground->setScaleY(0.7);
		this->addChild(_chipBackground, 4);

		_statusText = Text::create("", "Arial", 20);
		_statusText->setAnchorPoint(Vec2(0.f, 0.f));
		_statusText->setPosition(Vec2(size.width*0.25, size.height*0.15));
		_statusText->setColor(Color3B(255, 255, 0));
		this->addChild(_statusText, 4);

		for(int i = 0; i < HAND_CARD_COUNT; i++)
		{	
			_handCard[i] = PokerCard::create(0x00);
			_handCard[i]->setCardValue(0);
			_handCard[i]->setScale(0.86f);
			_handCard[i]->setAnchorPoint(Vec2(0.5, 0.5));
			
			this->addChild(_handCard[i], i);
		}


		setHandCardPosition();

		hideHandCard(false);

		return true;
	}

	void PlayerUI::setHandCardPosition()
	{
		Size size = _bg->getContentSize();
		Vec2 pos_card;
		switch (_seatNo)
		{
		case 0:
			pos_card =  Vec2(0, size.height*1.3);
			break;
		case 1:
			pos_card =  Vec2(0, size.height*1.3);
			break;
		case 2:
			pos_card =  Vec2(size.width*1.5, size.height*0.78);
			break;
		case 3:
			pos_card =  Vec2(size.width*1.5, size.height*0.25);
			break;
		case 4:
			pos_card =  Vec2(-size.width*1.4, size.height*0.25);
			break;
		case 5:
			pos_card =  Vec2(-size.width*1.4, size.height*0.78);
			break;
		default:
			break;
		}

		//调整牌的位置及大小
		for(int i = 0; i < HAND_CARD_COUNT; i++)
		{			
			Vec2 pos_temp =  pos_card + Vec2(size.width*i*0.34, 10);
			_handCard[i]->setPosition(pos_temp);
			_handCard[i]->setScale(0.86f);
		}		
	}

	void PlayerUI::setTableUI(GameTableUI* tableUI)
	{
		_TableUI = tableUI;
	}

	void PlayerUI::setName(const char* name)
	{
		_name = name;
		_nameText->setString(name);
		_nameText->setColor(Color3B(240,240,240));
	}

	void PlayerUI::setChip(LLONG chip)
	{
		_chip = chip;
		char tmp[50] = {0};
		sprintf(tmp, "%lld", _chip);
		_chipText->setString(tmp);
	}

	void PlayerUI::setStatus(const char* status,int value)
	{
		_statusText->setString(status);
		_statusText->setColor(Color3B(201,207,80));
		return ;
	}

	void PlayerUI::resetName(Node* pNode)
	{
		Text* text = (Text*)pNode;
		text->setString(_name);
		text->setColor(Color3B(240, 240, 240));
	}

	std::string PlayerUI::getName() const
	{
		return _name;
	}

	std::string PlayerUI::getHeadImage() const
	{
		return _headImage;
	}

	void PlayerUI::setSex(bool men, BYTE logicSeatNo)
	{
		int number = logicSeatNo % 5;
		Size size = _bg->getContentSize();
		_headImage = StringUtils::format(men ? MEN_HEAD : WOMEN_HEAD, number);
		_headSp = HNSprite::create(_headImage);
		_headSp->setAnchorPoint(Vec2(0.5f, 0.5f));
		_headSp->ignoreAnchorPointForPosition(false);
		_headSp->setPosition(size.width / 2, size.height / 2);
	
		this->addChild(_headSp, 3);
	}

	HNSprite* PlayerUI::getHeadSp()
	{
		return _headSp;
	}

	PokerCard* PlayerUI::getHandCard(int index)
	{
		return _handCard[index];
	}

	void PlayerUI::note(LLONG money)
	{
		char tmp[100] = {0};
		sprintf(tmp, "%lld", money);
		//_nameText->setString(tmp);
		//_nameText->setColor(Color3B(201,207,80));
		_statusText->setString(tmp);
		_statusText->setColor(Color3B(201,207,80));
	}

	void PlayerUI::profit(LLONG money)
	{
		char tmp[100] = {0};
		if(money > 0)
		{
			sprintf(tmp, "+%lld", money);
		}
		else
		{
			sprintf(tmp, "%lld", money);
		}
		_statusText->setString(tmp);
		_statusText->setColor(Color3B(201,207,80));
	}

	void PlayerUI::playAction(Action action)
	{
		//auto userid = _tableLogic->getUserId(seatNo);
		auto userinfo = UserInfoModule()->findUser(_userId);
		// true-man, false-lady
		auto sex = userinfo->bBoy;

		std::string audio("");
		std::string filename("");

		switch (action)
		{
		case PlayerUI::eLookCard:
			audio = sex ? GOLD_LOOK_MAN : GOLD_LOOK_LADY;
			filename.append("goldenflower/table/look_action.png");
			break;
		case PlayerUI::eGiverUp:
			audio = sex ? GOLD_GIVEUP_MAN : GOLD_GIVEUP_LADY;
			filename.append("goldenflower/table/giveup_action.png");
			break;
		case PlayerUI::eNote:
			audio = sex ? GOLD_ADD_MAN : GOLD_ADD_LADY;
			filename.append("goldenflower/table/note_action.png");
			break;
		case PlayerUI::eFollow:
			audio = sex ? GOLD_FOLLOW_MAN : GOLD_FOLLOW_LADY;
			filename.append("goldenflower/table/follow_action.png");
			break;
		default:
			break;
		}

		if (!audio.empty())
		{
			HNAudioEngine::getInstance()->playEffect(audio.c_str());
		}

		if (!filename.empty())
		{
			auto sp = Sprite::create(filename);
			sp->setPosition(_headSp->getContentSize().width / 2, _headSp->getContentSize().height / 2);
			addChild(sp, 10);
			sp->setGlobalZOrder(10);
			sp->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(0, 100)), DelayTime::create(1.5f), RemoveSelf::create(true), nullptr));
		}

	}

	void PlayerUI::setHandCard(int index, BYTE value)
	{
		if(index == 0 || index == 1 || index == 2)
		{
			_handCard[index]->setCardValue(value);
			_handCard[index]->setVisible(true);
			_handCard[index]->setRotation(0);

			if(value != 0x00)
			{	
				int width = _handCard[index]->getContentSize().width;
				this->reorderChild(_handCard[index], 4);	
			}
			else
			{
				int width = _handCard[index]->getContentSize().width;
				this->reorderChild(_handCard[index], 0);
			}
		}
	}

	void PlayerUI::changeHandCardValue(Node* pNode,int index, BYTE value)
	{
		if (index == 0 || index == 1 || index == 2)
		{
			_handCard[index]->setCardValue(value);
			_handCard[index]->setVisible(true);
			_handCard[index]->setRotation(0);

			if (value != 0x00)
			{
				int width = _handCard[index]->getContentSize().width;
				this->reorderChild(_handCard[index], 4);
			}
			else
			{
				int width = _handCard[index]->getContentSize().width;
				this->reorderChild(_handCard[index], 0);
			}
		}
	}

	// show the handcards for other players
	void PlayerUI::lookHandCard()
	{
		_handCard[0]->runAction(RotateBy::create(0.2f, 0));
		_handCard[1]->runAction(RotateTo::create(0.2f, 15));
		_handCard[2]->runAction(RotateTo::create(0.2f, 30));
		return;
	}

	// lose the combat
	void PlayerUI::lose()
	{
		_isQuit = true;
		for (int i = 0; i < HAND_CARD_COUNT; i++)
		{
			_handCard[i]->setCardValue(0x00);
			_handCard[i]->setGray(true);
		}
		return;
	}

	// set hand card broken when lose in card comparison
	void PlayerUI::setHandCardBroken()
	{
		for (int i = 0; i < HAND_CARD_COUNT; i++)
		{
			_handCard[i]->setCardValue(0x3E);
		}
		return;
	}

	void PlayerUI::hideHandCard(bool setVisible)
	{
		for (int i = 0; i < 3; i++)
		{
			if (_handCard[i])
			{
				_handCard[i]->setVisible(setVisible);
			}
		}
	}

	void PlayerUI::startWait(int time, int total)
	{
		if(time > 1 && total > 1)
		{
			_leftTime = time;
			_totalTime = total;
			schedule(schedule_selector(PlayerUI::onWaitTimer),0.1f);
		}
	}

	void PlayerUI::stopWait()
	{
		_progressTimer->setVisible(false);
		unschedule(schedule_selector(PlayerUI::onWaitTimer));
	}

	void PlayerUI::playWin(int type)
	{
		char tmp[50] = {0};
		sprintf(tmp,"cardtype-%d.png", type);
		auto sp = Sprite::createWithSpriteFrameName(tmp);
		if(sp == nullptr)
		{
			return;
		}
		sp->setPosition(_bg->getContentSize().width / 2, _bg->getContentSize().height / 2);
		this->addChild(sp,6,"winSprite");
		sp->runAction(Sequence::create(
			EaseIn::create(MoveBy::create(0.5f, Vec2(0, 50)), 2.5f),
			DelayTime::create(8.0f),
			RemoveSelf::create(true),
			nullptr));
	}

	void PlayerUI::stopPlayWin()
	{
		auto sp  = this->getChildByName("winSprite");
		if (nullptr == sp)
		{
			return ;
		}
		sp->stopAllActions();
		sp->removeFromParent();
	}

	void PlayerUI::onWaitTimer(float delta)
	{
		_leftTime -= 0.1f;
		if(_leftTime < 0)
		{
			stopWait();

			_TableUI->getReady();

			_progressTimer->setVisible(false);
			return;
		}

		Color3B color(0,255,0);//GREEN
		_progressTimer->setVisible(true);
		if (0 == _seatNo)
		{
			_progressTimer->setVisible(false);
		}

		float ratio = _leftTime / _totalTime;
		int value = ratio * 510;
		if(value > 255)
		{
			color.r  = 510 - value;
		}
		else
		{
			color.r = 255;
			//颜色变淡效果
			//color.g = value;
		}

		_progressTimer->setColor(color);
		_progressTimer->setPercentage(ratio * 100);
	}

	bool PlayerUI::onTouchBegan(Touch *touch, Event *unused_event)
	{
		if(_bg == nullptr)
		{
			return false;
		}
		Rect rect = _bg->getBoundingBox();
		Vec2 pos = this->convertToNodeSpace(touch->getLocation());
		if(rect.containsPoint(pos))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void PlayerUI::onTouchEnded(Touch *touch, Event *unused_event)
	{
		if(_bg != nullptr)
		{
			Rect rect = _bg->getBoundingBox();
			Vec2 pos = this->convertToNodeSpace(touch->getLocation());
			if(rect.containsPoint(pos))
			{
				auto playerInfo = GameUserMessageBox::createMessageBox(_TableUI, _userId, _headImage);
				playerInfo->setHidePoint(this->getPosition());
				_TableUI->reorderChild(playerInfo, this->getLocalZOrder() + 100);
				if(playerInfo != nullptr)
				{
					playerInfo->show();
				}
			}
		}
	}

	void PlayerUI::setViewSeatNo(BYTE seatNo)
	{
		_seatNo = seatNo;
	}

	bool PlayerUI::isPlaying()
	{
		return _isPlaying;
	}

	void PlayerUI::enterGame()
	{
		_isPlaying = true;
		_isQuit = false;
	}

	bool PlayerUI::isQuit()
	{
		return _isQuit;
	}

	LLONG PlayerUI::getUserID()
	{
		return _userId;
	}
}

	

