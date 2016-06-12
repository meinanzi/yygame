#include "BJLNotice.h"
#include "cocostudio/CocoStudio.h"
#include "HNLobby/FontConfig.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

namespace BJL
{

	static const int HEIGHT_HIDE = 122;    //提示框掩体的高度
	static const char* NOTICE_BG_SRC  = "BJL/game/noticemessege/noticemessege/control.png";

	GameNotice::GameNotice()
		:_bg(nullptr)
	{

	}

	GameNotice::~GameNotice()
	{

	}

	GameNotice* GameNotice::create(cocos2d::Node* parent, std::string context, cocos2d::Node* shelter , std::string title, std::string fileName )
	{
		GameNotice* ptr = new GameNotice();
		if (nullptr != ptr && ptr->init(parent,context, shelter ,title,fileName))
		{
			ptr->autorelease();
			return ptr;
		}
		return nullptr;
	}

	bool GameNotice::init(cocos2d::Node* parent, std::string& context, cocos2d::Node* shelter ,std::string& title, std::string& fileName)
	{
		if (!HNLayer::init())
		{
			return false;
		}

		_parent = parent;
		_shelter = shelter;
		HNSprite* sp = HNSprite::create(NOTICE_BG_SRC);
		sp->setRotation(180);
		Size size = sp->getContentSize();
		this->ignoreAnchorPointForPosition(false);
		this->setContentSize(size);

		sp->setAnchorPoint(Vec2(0.5f, 0.5f));
		sp->setPosition(size.width / 2, size.height / 2);
		sp->ignoreAnchorPointForPosition(false);
		addChild(sp);
		_bg = sp;
		auto contextLable = Label::createWithSystemFont(context, "", 22);
		contextLable->setAnchorPoint(Vec2(0.5f, 0.5f));
		contextLable->setPosition(Vec2(size.width / 2, size.height / 2));
		this->addChild(contextLable);
		return true;
	}

	void GameNotice::show()
	{
		auto winSize = Director::getInstance()->getWinSize();

		Vec2 end_position, start_position;
		if (nullptr != _shelter)
		{
			auto size= _shelter->getContentSize();
			float f_scaley = _shelter->getParent() ->getScaleY();
			auto rt = _shelter->getBoundingBox();
			float maxyy = rt.getMaxY();
			float maxY = _shelter->getBoundingBox().size.height;
			Vec2 pos_des = Vec2( rt.size.width / 2 , maxY - 13);
			start_position = pos_des;
			end_position = Vec2(start_position.x , start_position.y + _bg->getContentSize().height);
		}
		else
		{
			start_position = Vec2(winSize.width / 2, 0);
			end_position = Vec2(winSize.width / 2,  _bg->getContentSize().height);
		}


		this->setAnchorPoint(Vec2(0.5f,1));
		this->setPosition(start_position);
		_parent->addChild(this);

		auto come = EaseIn::create(MoveTo::create(0.3f, end_position), 1.5f);
		auto leave = EaseOut::create(MoveTo::create(0.3f, start_position), 1.5f);
		auto seq = Sequence::create(come, DelayTime::create(3.0f), leave, RemoveSelf::create(true), nullptr);
		this->runAction(seq);
	}

	bool GameNotice::loadUI(std::string& context, std::string& title, std::string& fileName)
	{
		return true;
	}


}
