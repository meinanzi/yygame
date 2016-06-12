#include "HNLobby/ChildLayer/GamePasswordInput.h"
#include "HNLobby/GamePrompt.h"
#include "HNNetExport.h"

GamePasswordInput::GamePasswordInput()
	: _editbox_pass(nullptr)
	, onEnterCallback(nullptr)
{

}

GamePasswordInput::~GamePasswordInput()
{

}

GamePasswordInput* GamePasswordInput::create()
{
	GamePasswordInput* room = new GamePasswordInput();
	if(room->init())
	{
		room->autorelease();
		return room;

	}
	CC_SAFE_DELETE(room);
	return nullptr;
}

bool GamePasswordInput::init()
{
	if ( !HNLayer::init()) 
	{
		return false;
	}
	ignoreAnchorPointForPosition(false);

	Size winSize = Director::getInstance()->getWinSize();

	//屏蔽后面的层
	auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
	addChild(colorLayer);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch ,Event* event){
		return true;
	};
	touchListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,colorLayer);

	auto csLoader = CSLoader::createNode("platform/passwordInput/passwordInput.csb");
	this->addChild(csLoader);
	csLoader->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

	auto button_close = dynamic_cast<ui::Button*>(csLoader->getChildByName("Button_close"));
	button_close->addTouchEventListener(CC_CALLBACK_2(GamePasswordInput::onCloseClickCallback, this));

	auto button_enter = dynamic_cast<ui::Button*>(csLoader->getChildByName("Button_enter"));
	button_enter->addTouchEventListener(CC_CALLBACK_2(GamePasswordInput::onEnterClickCallback, this));

	auto editbox_pass = dynamic_cast<ui::TextField*>(csLoader->getChildByName("TextField_password"));
	editbox_pass->setVisible(false);
	_editbox_pass = HNEditBox::createEditBox(editbox_pass, this);
	_editbox_pass->setPasswordEnabled(true);

	return true;
}

void GamePasswordInput::onCloseClickCallback(Ref* pSender, Widget::TouchEventType type)
{
	if(type != Widget::TouchEventType::ENDED)
	{
		return;
	}

	this->runAction(Sequence::create(
		ScaleTo::create(0.1f, 0.1f), 
		CCCallFunc::create([&](){
			this->removeFromParent();
		}), nullptr));
}

void GamePasswordInput::onEnterClickCallback(Ref* pSender, Widget::TouchEventType type)
{
	if(type != Widget::TouchEventType::ENDED)
	{
		return;
	}

	std::string pass = _editbox_pass->getString();
	if(!pass.empty())
	{
		if(onEnterCallback != nullptr)
		{
			onEnterCallback(pass);
			this->runAction(Sequence::create(
				ScaleTo::create(0.1f, 0.1f), 
				CCCallFunc::create([&](){
					this->removeFromParent();
			}), nullptr));
		}
	}
	else
	{
		GamePromptLayer::create()->showPrompt(GBKToUtf8("请输入房间密码"));
	}
}


