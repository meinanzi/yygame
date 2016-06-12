#include "TRACTORGameNotice.h"
#include "cocostudio/CocoStudio.h"
#include "HNLobbyExport.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
namespace TRACTOR
{

	static const CHAR* NOTICE_BG_SRC	=	 "tractor/noticemessege/notice_bg.png";

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

		_parent = parent;
		HNSprite* sp = HNSprite::create(NOTICE_BG_SRC);
		//sp->setScale(0.5f);
		Size size = sp->getContentSize();
		this->ignoreAnchorPointForPosition(false);
		this->setContentSize(size);

		sp->setAnchorPoint(Vec2(0.5f, 0.5f));
		sp->setPosition(size.width / 2, size.height / 2);
		sp->ignoreAnchorPointForPosition(false);
		addChild(sp, 0);
		_bg = sp;

		auto contextLable = Label::createWithSystemFont(noticeMsg, "", 22);
		contextLable->setAnchorPoint(Vec2(0.5f, 0.5f));
		contextLable->setPosition(Vec2(size.width / 2, size.height / 2));
		addChild(contextLable, 1);

		return true;
	}

	void GameNotice::show()
	{
		auto winSize = Director::getInstance()->getWinSize();
		this->setPosition(winSize.width / 2, winSize.height * 0.5f);
		this->setScale(0);
		_parent->addChild(this);

		auto come  = ScaleTo::create(0.3f, 1.0f);
		auto leave = ScaleTo::create(0.3f, 0);
		auto seq = Sequence::create(come, DelayTime::create(3.0f), leave, RemoveSelf::create(true), nullptr);
		this->runAction(seq);
	}
}
