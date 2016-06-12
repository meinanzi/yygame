#include "LandlordGameNotice.h"
#include "cocostudio/CocoStudio.h"
#include "HNLobbyExport.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

namespace landlord
{

#define Notice_Bg         "landlord/game/studioUI/uiRes/notice_bg.png"
#define Notice_Bg_Stencil "landlord/game/studioUI/uiRes/notice_bg_stencil.png"
#define Offset_Width      10

	GameNotice::GameNotice()
		:_bg(nullptr)
	{

	}

	GameNotice::~GameNotice()
	{

	}

	GameNotice* GameNotice::create(cocos2d::Node* parent, std::string noticeMsg)
	{
		GameNotice* ptr = new GameNotice();
		if (nullptr != ptr && ptr->init(parent,noticeMsg))
		{
			ptr->autorelease();
			return ptr;
		}
		return nullptr;
	}

	bool GameNotice::init(cocos2d::Node* parent, std::string& noticeMsg)
	{
		if (!HNLayer::init())
		{
			return false;
		}

		//330 ,40

		_parent = parent;
		HNSprite* sp = HNSprite::create(Notice_Bg);
		_showSize = sp->getContentSize();
		this->ignoreAnchorPointForPosition(false);
		this->setContentSize(_showSize);

		sp->setAnchorPoint(Vec2::ZERO);
		sp->setPosition(Vec2::ZERO);
		addChild(sp, 0);
		_bg = sp;

		std::string str(GBKToUtf8(noticeMsg.c_str()));
		_text = Label::createWithSystemFont(str, "", 22);
		_text->setAnchorPoint(Vec2(0.5f, 0.5f));
		_text->setPositionY(_showSize.height / 2);
		_wordSize = _text->getContentSize();

		if(_wordSize.width > _showSize.width)
		{
			_text->setPositionX(_wordSize.width / 2 + Offset_Width);
		}
		else
		{
			_text->setPositionX(_showSize.width / 2);
		}

		auto stencil = HNSprite::create(Notice_Bg_Stencil);
		stencil->setAnchorPoint(Vec2(0.5f, 0.5f));
		stencil->setPosition(_showSize.width / 2, _showSize.height / 2);
		ClippingNode* clipNode = ClippingNode::create();
		clipNode->setStencil(stencil);
		clipNode->setAnchorPoint(Vec2::ZERO);
		clipNode->setPosition(Vec2::ZERO);
		this->addChild(clipNode);

		clipNode->addChild(_text);


		this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create(CC_CALLBACK_0(GameNotice::start, this)),nullptr));

		return true;
	}

	void GameNotice::moveWord(float delta)
	{	
		if(_wordSize.width > _showSize.width)
		{
			float posX = _text->getPositionX() - 1;
			float overPosX = 0;
			overPosX = _showSize.width - _wordSize.width / 2 - Offset_Width;
			if(posX <= overPosX)
			{
				stop();
			}
			else
			{
				_text->setPositionX(posX);
			}
		}
		else
		{
			stop();
		}
	}

	void GameNotice::start()
	{
		schedule(schedule_selector(GameNotice::moveWord), 0.02f);
	}

	void GameNotice::stop()
	{
		unschedule(schedule_selector(GameNotice::moveWord));
		auto sequence = Sequence::create(DelayTime::create(2.0f), ScaleTo::create(0.3f, 0), RemoveSelf::create(true), nullptr);
		this->runAction(sequence);
	}

	void GameNotice::show()
	{
		auto winSize = Director::getInstance()->getWinSize();
		this->setPosition(winSize.width / 2, winSize.height - 80);
		this->setScale(0);
		_parent->addChild(this);

		auto come  = ScaleTo::create(0.3f, 1.0f);
		//auto leave = ScaleTo::create(0.3f, 0);
		auto seq = Sequence::create(come, DelayTime::create(3.0f)/*, leave, RemoveSelf::create(true)*/, nullptr);
		this->runAction(seq);
	}
}


