#include "DZPokerGamePlayer.h"
#include "DZPokerGameUserMessageBox.h"
#include "DZPokerGameClientMessage.h"

#include "HNCommon/HNConverCode.h"
#include "HNLobbyExport.h"

namespace DZPoker
{

	static const int BUTTON_TAG_CHECK	=	100	;		//100时表示看牌
	static const int  BUTTON_TAG_CALL	=	101	;		//101时表示跟注
	static const int  BUTTON_TAG_ALLIN	=	102	;		//102时表示全下

	static const int  BUTTON_TAG_BET	=	201	;		//201时表示下注
	static const int  BUTTON_TAG_RAISE	=	202	;		//202时表示加注

	static const int  BUTTON_TAG_FLOD	=	301	;		//301时表示弃牌

	static Color3B USER_NAME_COLOR(240, 240, 240);		// 玩家名称颜色

	static Color3B USER_CHIPTEXT_COLOR(122,162,116);	// 玩家筹码文本颜色

	static Color3B USER_STATUS_COLOR(201,207,80);		// 玩家状态颜色

	static const char* WOMEN_HEAD =  "dzpoker/table/women_head.png";
	static const char* MEN_HEAD =  "dzpoker/table/men_head.png";
	static const char* BG_SRC  = "dzpoker/table/seat.png";
	static const char* BG_LINE_SRC  = "dzpoker/table/status.png";
	static const char* PROGRESS_SRC  = "dzpoker/table/white_frame.png";

	PlayerUI* PlayerUI::create(LLONG userId)
	{
		PlayerUI* player = new PlayerUI();
		if(player && player->init(userId))
		{
			player->setScale(1.3f);
			player->autorelease();
			return player;
		}
		CC_SAFE_DELETE(player);
		return nullptr;
	}

	bool PlayerUI::init(LLONG userId)
	{
		if(!HN::HNLayer::init())
		{
			return false;
		}
		_userId = userId;

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
		this->addChild(_progressTimer,3);
		_progressTimer->setScaleX(size.width / timerSp->getContentSize().width);
		_progressTimer->setScaleY(size.height / timerSp->getContentSize().height);
		_progressTimer->setPosition(size.width / 2, size.height / 2);
		_progressTimer->setPercentage(0);
		_progressTimer->setVisible(false);
		_progressTimer->setReverseProgress(true);

		auto bgLine = HNSprite::create(BG_LINE_SRC);
		this->addChild(bgLine,2);
		bgLine->setAnchorPoint(Vec2(0.5f,0.5f));
		bgLine->setPosition(size.height + (size.width - size.height) / 2, size.height / 2);

		_bg->setAnchorPoint(Vec2::ZERO);
		_bg->setPosition(Vec2::ZERO);

		_nameText = Text::create("", "Arial", 14);
		_nameText->setAnchorPoint(Vec2(0.5f, 0.5f));
		_nameText->setPosition(Vec2((size.width - size.height) / 2 + size.height, size.height / 3 * 2));
		_nameText->setColor(USER_NAME_COLOR);
		this->addChild(_nameText,3);

		_chipText = Text::create("", "Arial", 16);
		_chipText->setAnchorPoint(Vec2(0.5f, 0.5f));
		_chipText->setPosition(Vec2((size.width - size.height) / 2 + size.height, size.height / 3));
		_chipText->setColor(USER_CHIPTEXT_COLOR);
		this->addChild(_chipText,3);

		_handCard[0] = PokerCard::create();
		_handCard[0]->setCardValue(0);
		_handCard[0]->setCardFrameName(_handCard[0]->getCardFileName());
		_handCard[0]->setPosition(size.width / 2 - 5, size.height + 5);
		_handCard[0]->setScale(0.75f);
		this->addChild(_handCard[0], 0);

		_handCard[1] = PokerCard::create();
		_handCard[1]->setCardValue(0);
		_handCard[1]->setCardFrameName(_handCard[1]->getCardFileName());
		_handCard[1]->setPosition(size.width / 2 + 15, size.height);
		_handCard[1]->setScale(0.75f);
		this->addChild(_handCard[1], 0);

		hideHandCard();

		return true;
	}

	PlayerUI::PlayerUI()
		: _chip(0)
		, _userId(0)
		, _leftTime(0)
		, _bg(nullptr)
		, _headSp(nullptr)
		, _nameText(nullptr)
		, _chipText(nullptr)
		, _progressTimer(nullptr)
	{
		memset(_handCard, 0, sizeof(_handCard));
	}

	PlayerUI::~PlayerUI()
	{
		unschedule(schedule_selector(PlayerUI::onWaitTimer));
	}

	void PlayerUI::setName(const std::string& name)
	{
		_name = name;
		_nameText->setString(name);
		_nameText->setColor(USER_NAME_COLOR);
	}

	void PlayerUI::setSex(bool men)
	{
		Size size = _bg->getContentSize();
		_headSp = HNSprite::create(men ? MEN_HEAD: WOMEN_HEAD);
		_headSp->setScale(0.6f);
		_headSp->setAnchorPoint(Vec2(0.5f, 0.5f));
		_headSp->ignoreAnchorPointForPosition(false);
		_headSp->setPosition(size.height / 2, size.height / 2);
		this->addChild(_headSp, 3);
	}

	void PlayerUI::setChip(LLONG chip)
	{
		_chip = chip;
		char tmp[128] = {0};
		sprintf(tmp, "%lld", _chip);
		_chipText->setString(tmp);
		_chipText->runAction(Sequence::create(ScaleTo::create(0.5f, 1.5f), ScaleTo::create(0.5f, 1.0f), nullptr));
	}

	void PlayerUI::setStatus(const char* status, int value)
	{
		_nameText->setString(status);
		_nameText->setColor(USER_STATUS_COLOR);
		return ;				//yanghuanSearch
		//_nameText->runAction(Sequence::create(DelayTime::create(2.0f), CallFuncN::create(CC_CALLBACK_1(PlayerUI::resetName,this)) ,nullptr));
		char str[50]={0};
		sprintf(str,"table/table_playerHandle-%d.png",value);
		auto sp = Sprite::create("table/table_playerHandle.png");
		if(sp == nullptr)
		{
			return;
		}
		sp->setPosition(0, _bg->getContentSize().height / 2);
		this->addChild(sp,5);

		auto text = createLabel(status, 25);
		auto size = sp->getContentSize();
		text->setAnchorPoint(Vec2(0.5f,1));
		//text->setColor(Color3B::RED);
		text->setPosition(Vec2(size.width*0.5 , size.height-2));
		sp->addChild(text);
		sp->setScale(0);
		auto seq = Sequence::create(
			Spawn::create(
			ScaleTo::create(1,1),
			EaseOut::create(MoveBy::create(1, Vec2(0, 50)), 1),
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
		text->setColor(Color3B(240,240,240));
	}

	std::string PlayerUI::getName() const
	{
		return _name;
	}

	void PlayerUI::setHandCard(int index, BYTE value)
	{
		if (index == 0 || index == 1)
		{
			_handCard[index]->setCardValue(value);
			_handCard[index]->setCardFrameName(_handCard[index]->getCardFileName());
			_handCard[index]->setVisible(true);
			if(value != 0x00)
			{
				this->reorderChild(_handCard[index], 0);
				_handCard[index]->setPositionY(_handCard[index]->getPositionY());
			}
			else
			{
				this->reorderChild(_handCard[index], 0);
			}
		}
	}

	void PlayerUI::hideHandCard()
	{
		_handCard[0]->setVisible(false);
		_handCard[1]->setVisible(false);
		if (_handCard[0]->getCardValue() != 0x00 && _handCard[1]->getCardValue() != 0x00)
		{
			_handCard[0]->setPositionY(_handCard[0]->getPositionY());
			_handCard[1]->setPositionY(_handCard[1]->getPositionY());
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
			EaseOut::create(MoveBy::create(0.5f, Vec2(0, 50)), 2.5f),
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
				playerInfo->setHidePoint(this->getPosition());
				this->getParent()->reorderChild(playerInfo, MAX_ZORDER);
				if(playerInfo != nullptr)
				{
					playerInfo->show();
				}
			}
		}
	}

}
