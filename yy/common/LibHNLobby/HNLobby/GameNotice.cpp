#include "HNLobby/GameNotice.h"
#include "HNLobby/FontConfig.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

#define Offset_Width      10

GameNotice::GameNotice()
	: _text(nullptr)
	, _runing(false)
{

}

GameNotice::~GameNotice()
{

}

GameNotice* GameNotice::createGameNotice(const char* stencil_Path)
{
	GameNotice* notice = new GameNotice();
	if (notice->init(stencil_Path))
	{
		notice->autorelease();
		return notice;
	}
	else
	{
		CC_SAFE_DELETE(notice);
		return nullptr;
	}
}

bool GameNotice::init(const char* stencil_Path)
{
	if (!HNLayer::init())
	{
		return false;
	}

	auto stencil = HNSprite::create(stencil_Path);
	_showSize = stencil->getContentSize();

	stencil->setAnchorPoint(Vec2(0.5f, 0.5f));
	stencil->setPosition(_showSize.width / 2, _showSize.height / 2);

	_text = createLabel("", 18);
	_text->setAnchorPoint(Vec2(0.5f, 0.5f));
	_text->setPositionY(_showSize.height / 2);
	_wordSize = _text->getContentSize();
	_text->setPositionX(_showSize.width + _wordSize.width / 2);

	ClippingNode* clipNode = ClippingNode::create();
	clipNode->setStencil(stencil);
	clipNode->setAnchorPoint(Vec2::ZERO);
	clipNode->setPosition(Vec2::ZERO);
	this->addChild(clipNode);
	clipNode->addChild(_text);
	clipNode->setCascadeOpacityEnabled(true);

	this->ignoreAnchorPointForPosition(false);
	this->setContentSize(_showSize);

	return true;
}

void GameNotice::postMessage(const std::string& message, int repeat)
{
	_notifyMsgs.push(NotifyItem(message, repeat));
	
	if (!_notifyMsgs.empty())
	{
		start();
	}
}

void GameNotice::moveWord(float delta)
{	

	if (nullptr == _text) return;
	
	if (_text->getString().empty())
	{
		updateNextMessage();
	}
	else
	{
		float posX = _text->getPositionX() - 2;
		float overPosX = 0;
		overPosX = -_wordSize.width / 2;
		if(posX <= overPosX)
		{
			updateNextMessage();
		}
		else
		{
			_text->setPositionX(posX);
		}
	}
}

void GameNotice::start()
{
	if (!_runing)
	{
		_runing = true;
		schedule(schedule_selector(GameNotice::moveWord), 0.01f);
	}
}

void GameNotice::stop()
{
	if (_runing)
	{
		unschedule(schedule_selector(GameNotice::moveWord));
		_runing = false;
		if (nullptr != _sure)
		{
			_sure();
		}
	}
}

void GameNotice::updateNextMessage()
{
	if (!_notifyMsgs.empty())
	{
		NotifyItem msg = _notifyMsgs.front();
		_text->setString(msg.message);
		_wordSize = _text->getContentSize();
		_text->setPositionX(_showSize.width + _wordSize.width / 2);
		if (msg.repeat-- > 0)
		{
			_notifyMsgs.pop();
			_notifyMsgs.push(msg);
		}
		else
		{
			_notifyMsgs.pop();
		}
	}
	else
	{
		stop();
	}
}

void GameNotice::setCallBack(std::function<void ()> sure)
{
	_sure = sure;
}

void GameNotice::setNoticTextColor(Color4B color)
{
	_text->setTextColor(color);
}

void GameNotice::setTextFontSize(int fontSize)
{
	_text->setSystemFontSize(fontSize);
}

