#include "TRACTORGamePlayer.h"
#include "HNCommon/HNConverCode.h"
#include "TRACTORGameTableUI.h"
#include "TRACTORGameUserMessageBox.h"
#include "HNLobbyExport.h"
#include "TRACTORPokerCard.h"

namespace TRACTOR
{
	/************************line*********************************/
	static const  char*    WOMEN_HEAD  =   "tractor/table/women_head.png";
	static const  char*    MEN_HEAD    =   "tractor/table/men_head.png";
	static const  char*   BG_SRC       =   "tractor/table/seat.png"			;
	static const  char*   BG_LINE_SRC  =   "tractor/table/status.png"		;
	static const  char*   PROGRESS_SRC =   "tractor/table/white_frame.png"	;
	static const  INT	  CARD_DISTANCE =	20;



	PlayerUI* PlayerUI::create(LLONG userId)
	{
		PlayerUI* player = new PlayerUI(userId);
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


	PlayerUI::PlayerUI(LLONG userId)
		: _chip(0)
		, _headSp(nullptr)
		, _nameText(nullptr)
		, _chipText(nullptr)
		, _bg(nullptr)
		, _progressTimer(nullptr)
		, _leftTime(0)
		, _userId(userId)
		,_TableUI(nullptr)
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

		auto bgLine = HNSprite::create(BG_LINE_SRC);
		this->addChild(bgLine, 2);
		bgLine->setAnchorPoint(Vec2(0.5f, 0.5f));
		bgLine->setPosition(size.height + (size.width - size.height) / 2, size.height / 2);

		_bg->setAnchorPoint(Vec2::ZERO);
		_bg->setPosition(Vec2::ZERO);



		_nameText = Text::create("", "Arial", 20);
		_nameText->setAnchorPoint(Vec2(0.5f, 0.5f));
		_nameText->setPosition(Vec2((size.width - size.height) / 2 + size.height, size.height / 3 * 2));
		_nameText->setColor(Color3B(240,240,240));
		this->addChild(_nameText, 3);

		_chipText = Text::create("", "Arial", 18);
		_chipText->setAnchorPoint(Vec2(0.5f, 0.5f));
		_chipText->setPosition(Vec2((size.width - size.height) / 2 + size.height, size.height / 3));
		_chipText->setColor(Color3B(122, 162, 116));
		this->addChild(_chipText, 3);

		for(int i = 0; i < HAND_CARD_COUNT; i++)
		{
			_handCard[i] = PokerCard::create(0x00);
			_handCard[i]->setCardValue(0);
			//_handCard[i]->setCardFrameName(_handCard[i]->getCardFileName());
			_handCard[i]->setPosition(size.width / 2 + (i - 1) * CARD_DISTANCE, size.height - 15);
			//_handCard[i]->setScale(0.64f);
			this->addChild(_handCard[i],i);
		}

		hideHandCard(false);

		return true;
	}


	void PlayerUI::setName(const char* name)
	{
		_name = name;
		_nameText->setString(GBKToUtf8(name));
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
		_nameText->setString(status);
		_nameText->setColor(Color3B(201,207,80));
		_nameText->runAction(Sequence::create(DelayTime::create(2.0f), CallFuncN::create(CC_CALLBACK_1(PlayerUI::resetName,this)) ,nullptr));
		return ;

		//not used bellow.
		char str[50]={0};
		sprintf(str,"tractor/table/table_playerHandle-%d.png",value);
		auto sp = Sprite::create("tractor/table/table_playerHandle.png");
		if(sp == nullptr)
		{
			return;
		}
		sp->setPosition(0, _bg->getContentSize().height / 2);
		this->addChild(sp,5);

		auto text = Label::createWithSystemFont(status, "", 22);
		auto size = sp->getContentSize();
		text->setAnchorPoint(Vec2(0.5f,1));
		//text->setColor(Color3B::RED);
		text->setPosition(Vec2(size.width*0.5 , size.height-2));
		sp->addChild(text);
		sp->setScale(0);
		auto seq = Sequence::create(
			Spawn::create(
			ScaleTo::create(1,1),
			EaseIn::create(MoveBy::create(1, Vec2(0, 50)), 1),
			nullptr
			),
			DelayTime::create(1.0f),
			RemoveSelf::create(true),
			nullptr
			);
		sp->runAction(seq);
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

	void PlayerUI::setSex(bool men)
	{
		Size size = _bg->getContentSize();
		_headSp = HNSprite::create(men ? MEN_HEAD : WOMEN_HEAD);
		_headSp->setScale(0.6f);
		_headSp->setAnchorPoint(Vec2(0.5f, 0.5f));
		_headSp->ignoreAnchorPointForPosition(false);
		_headSp->setPosition(size.height / 2, size.height / 2);
		this->addChild(_headSp, 3);

	}
	void PlayerUI::note(LLONG money)
	{
		char tmp[100] = {0};
		sprintf(tmp, "%lld", money);
		_nameText->setString(tmp);
		_nameText->setColor(Color3B(201,207,80));
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
		_nameText->setString(tmp);
		_nameText->setColor(Color3B(201,207,80));
	}

	void PlayerUI::playAction(Action action)
	{
		//auto userid = _tableLogic->getUserId(seatNo);
		auto userinfo = HNUserInfoModule::getInstance()->findUser(_userId);
		if (nullptr == userinfo)
		{
			return ;
		}
		// true-man, false-lady
		auto sex = userinfo->bBoy;

		std::string audio("");
		std::string filename("");

		switch (action)
		{
		case PlayerUI::eLookCard:
			audio = sex ? GOLD_LOOK_MAN : GOLD_LOOK_LADY;
			filename.append("tractor/table/look_action.png");
			break;
		case PlayerUI::eGiverUp:
			audio = sex ? GOLD_GIVEUP_MAN : GOLD_GIVEUP_LADY;
			filename.append("tractor/table/giveup_action.png");
			break;
		case PlayerUI::eNote:
			audio = sex ? GOLD_ADD_MAN : GOLD_ADD_LADY;
			filename.append("tractor/table/note_action.png");
			break;
			//case PlayerUI::eCompareCard:
			//	audio = sex ? GOLD_PK_MAN : GOLD_PK_LADY;
			//	filename.append("table/compare_action.png");
			//	break;
		case PlayerUI::eFollow:
			audio = sex ? GOLD_FOLLOW_MAN : GOLD_FOLLOW_LADY;
			filename.append("tractor/table/follow_action.png");
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
			sp->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(0, 50)), DelayTime::create(1.5f), RemoveSelf::create(true), nullptr));
		}

	}

	void PlayerUI::setHandCard(int index, BYTE value)
	{
		if(index == 0 || index == 1 || index == 2)
		{
			_handCard[index]->setCardValue(value);
			//_handCard[index]->setCardFrameName(_handCard[index]->getCardFileName());
			_handCard[index]->setVisible(true);
			_handCard[index]->setRotation(0);
			if(value != 0x00)
			{
				this->reorderChild(_handCard[index], 4);
			}
			else
			{
				this->reorderChild(_handCard[index], 0);
			}
		}
	}

	// show the handcards for other players
	void PlayerUI::lookHandCard()
	{
		_handCard[0]->runAction(RotateBy::create(0.2f, -15));
		_handCard[1]->runAction(RotateBy::create(0.2f, 15));
		_handCard[2]->runAction(RotateBy::create(0.2f, 30));
		return;
	}

	// lose the combat
	void PlayerUI::lose()
	{
		for (int i = 0; i < HAND_CARD_COUNT; i++)
		{
			_handCard[i]->setCardValue(0x00);
			_handCard[i]->setGray(true);
		}
		return;
	}

	void PlayerUI::hideHandCard(bool setVisible)
	{
		for(auto ptr: _handCard)
		{
			ptr->setVisible(setVisible);
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

		float ratio = _leftTime / _totalTime;
		int value = ratio * 510;
		if(value > 255)
		{
			color.r  = 510 - value;
		}
		else
		{
			color.r = 255;
			color.g = value;
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
			if(!rect.containsPoint(touch->getLocation()))
			{
				auto playerInfo = GameUserMessageBox::createMessageBox(this->getParent()->getParent(), _userId);
				playerInfo->setAnchorPoint(Vec2(0.5f , 0.5f));
				playerInfo->setHidePoint(this->getPosition());
				this->getParent()->reorderChild(playerInfo, this->getLocalZOrder() + 1);
				if(playerInfo != nullptr)
				{
					playerInfo->show();
				}
			}
		}
	}

	void PlayerUI::setTableUI(GameTableUI* tableUI)
	{
		_TableUI = tableUI;
	}
}