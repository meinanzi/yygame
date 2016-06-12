#include "WaterMarginGameBiBei.h"
#include "WaterMarginGameTableLogic.h"
#include "WaterMarginGameTableUI.h"
#include "WaterMarginGameAudio.h"
#include "cocostudio/CocoStudio.h"

#define Anm_WAIT 33

namespace WaterMargin
{
	GameBiBei::GameBiBei()
		:_father(nullptr)
		,_touchIndex(-1)
		,_Armature(nullptr)
		,_bossSprite(nullptr)
		,_leftSprite(nullptr)
		,_rightSprite(nullptr)
		, _gameAni(nullptr)
	{
		_myAnimation.count = 0;
		_myAnimation.name = "";
		_myAnimation.playCount = 0;
		_myAnimation.playtime = 0.0;
		_myAnimation.removeTime = 0.0;

		_pos[0] = Vec2(0, 73);
		_pos[1] = Vec2(-379, 67);
		_pos[2] = Vec2(382, 76);
	}

	GameBiBei::~GameBiBei()
	{
		CC_SAFE_DELETE(_gameAni);
	}

	GameBiBei* GameBiBei::create(GameTableLogic* tableLogic, Node* str)
	{
		GameBiBei* ptr = new GameBiBei();
		if (nullptr != ptr && ptr->init(tableLogic, str))
		{
			ptr->autorelease();
			return ptr;
		}
		return nullptr;
	}

	bool GameBiBei::init(GameTableLogic* tableLogic, Node* str)
	{
		_bibeiLogic = tableLogic;
		_father = str;

		_gameAni = new GameAnimation();

		winSize = Director::getInstance()->getWinSize();
		auto node = CSLoader::createNode("WaterMargin/tableui/tableui_1.csb");
		auto secondWidget = (Layout*)node->getChildByName("Panel_8");
		secondWidget->setAnchorPoint(Vec2(0.5, 0.5));
		secondWidget->setPosition(Vec2(640,360));
		_father->getParent()->addChild(node);
		secondWidget->setScale(winSize.width/1280, winSize.height/720);

		_secondBG = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(secondWidget, "bg"));
		auto size = _secondBG->getCustomSize();
		
		auto Text_bosslost = (Text*)_secondBG->getChildByName("Text_bosslost");
		_pos_bosslost = Vec2(Text_bosslost->getPosition());
		auto Text_bossopen = (Text*)_secondBG->getChildByName("Text_bossopen");
		_pos_bossopen = Vec2(Text_bossopen->getPosition());
		auto Text_bossthrow = (Text*)_secondBG->getChildByName("Text_bossthrow");
		_pos_bossthrow = Vec2(Text_bossthrow->getPosition());
		auto Text_bosswait = (Text*)_secondBG->getChildByName("Text_bosswait");
		_pos_bosswait = Vec2(Text_bosswait->getPosition());
		auto Text_bosswin = (Text*)_secondBG->getChildByName("Text_bosswin");
		_pos_bosswin = Vec2(Text_bosswin->getPosition());

		auto Text_leftlost = (Text*)_secondBG->getChildByName("Text_leftlost");
		_pos_leftlost = Vec2(Text_leftlost->getPosition());
		auto Text_leftwait = (Text*)_secondBG->getChildByName("Text_leftwait");
		_pos_leftwait = Vec2(Text_leftwait->getPosition());
		auto Text_leftwin = (Text*)_secondBG->getChildByName("Text_leftwin");
		_pos_leftwin = Vec2(Text_leftwin->getPosition());
		auto Text_rightlost = (Text*)_secondBG->getChildByName("Text_rightlost");
		_pos_rightlost = Vec2(Text_rightlost->getPosition());
		auto Text_rightwait = (Text*)_secondBG->getChildByName("Text_rightwait");
		_pos_rightwait = Vec2(Text_rightwait->getPosition());
		auto Text_rightwin = (Text*)_secondBG->getChildByName("Text_rightwin");
		_pos_rightwin = Vec2(Text_rightwin->getPosition());

		char name[30] = {0};

		for (int i = 0; i < 10; i++)
		{
			sprintf(name, "history_%d", i+1);
			auto history = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_secondBG, name));
			_history.push_back(history);
		}
		
		auto note = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_secondBG, "Note"));
		note->setVisible(false);
		note->setName("Note");
		
		//三个显示数据
		for (int i = 1; i < 4; i++)
		{
			sprintf(name, "score_%d", i);
			auto Bdata = dynamic_cast<TextAtlas*>(ui::Helper::seekWidgetByName(_secondBG, name));
			_Bdata.push_back(Bdata);
		}

		//大、小、和按钮
		for (int i = 1; i < 4; i++)
		{
			sprintf(name, "select_%d", i);
			auto btn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_secondBG, name));
			btn->addTouchEventListener(CC_CALLBACK_2(GameBiBei::menuClickCallback, this));
			btn->setVisible(false);
			_btn.push_back(btn);
		}

		//比倍、得分按钮
		for (int i = 1; i < 3; i++)
		{
			sprintf(name, "btn_%d", i);
			auto btn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_secondBG, name));
			btn->addTouchEventListener(CC_CALLBACK_2(GameBiBei::menuClickCallback, this));	
			_btn.push_back(btn);
		}

		//三个筹码显示
		for (int i = 0; i < 3; i++)
		{
			sprintf(name, "note_%d", i+1);
			auto note = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_secondBG, name));
			note->setVisible(false);
			_note.push_back(note);

		}
		//创建三个播放动画的精灵
		_aniB = Sprite::create();
		_secondBG->addChild(_aniB);
		_aniB->setAnchorPoint(Vec2(0.5f, 0.5f));

		//Right
		_aniR = Sprite::create();
		_secondBG->addChild(_aniR);
		_aniR->setAnchorPoint(Vec2(0.5f, 0.5f));
		
		//Left
		_aniL = Sprite::create();
		_secondBG->addChild(_aniL);
		_aniL->setAnchorPoint(Vec2(0.5f, 0.5f));
		
		//播放roll动画
		startPlay();

		return true;
	}

	bool GameBiBei::containRectTouch()
	{
		if (_touchIndex < 0)	return false;
		for (int i = 0; i < 3; i++)
		{
			auto pos = _note[i]->getPosition();
			auto size = _note[i]->getCustomSize();
			
			Rect rect = Rect(pos.x-size.width/2, pos.y-size.height/2, pos.x+size.width/2, pos.y+size.height/2);
			if (rect.containsPoint(_touchDown) && !_note[i]->isVisible())
			{
				_touchpos.push_back(i);
				return true;
			}
		}
		return false;		
	}

	bool GameBiBei::onTouchBegan(Touch *touch, Event *unused_event)
	{
		_touchDown = Director::getInstance()->convertToGL(touch->getLocationInView());
		if (_secondBG != nullptr)
		{
			return containRectTouch();
		}
		return false;
	}
	void GameBiBei::onTouchMoved(Touch *touch, Event *unused_event)
	{

	}
	void GameBiBei::onTouchEnded(Touch *touch, Event *unused_event)
	{
		_touchDown = Director::getInstance()->convertToGL(touch->getLocationInView());
		if (containRectTouch())
		{
			if (_touchpos[0] == _touchpos[1])
			{
				_note[_touchpos[0]]->setVisible(true);
				BiBeiOver();
				if (_touchpos[0] == 2)
				{
					_bibeiLogic->sendDaXiaoMessage(0,0);
				}
				else
				{
					_bibeiLogic->sendDaXiaoMessage(0,_touchpos[0]+1);
				}
				_touchIndex = -1;
				_touchpos.clear();
			}
		}
	}

	void GameBiBei::viewSaiZi(int s1, int s2)
	{
		char name[30] = {0};
		sprintf(name, "WaterMargin/useImage/%d.png", s1);
		
		auto pos1 = dynamic_cast<ImageView*>(_secondBG->getChildByName("saizi_1"));
		pos1->loadTexture(name);
		_saizi.push_back(pos1);

		sprintf(name, "WaterMargin/useImage/%d.png", s2);
		
		auto pos2 = dynamic_cast<ImageView*>(_secondBG->getChildByName("saizi_2"));
		pos2->loadTexture(name);
		_saizi.push_back(pos2);
	}

	void GameBiBei::menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (Widget::TouchEventType::ENDED != touchtype)	return;
		Button* ptr = (Button*)pSender;
		switch (ptr->getTag())
		{
		case 241:
			_note[2]->setVisible(true);
			_bibeiLogic->sendDaXiaoMessage(0,0);//da
			BiBeiOver();
			break;
		case 242:
			_note[0]->setVisible(true);
			_bibeiLogic->sendDaXiaoMessage(0,1);//xiao
			BiBeiOver();
			break;
		case 243:
			_note[1]->setVisible(true);
			_bibeiLogic->sendDaXiaoMessage(0,2);//he
			BiBeiOver();
			break;
		case 244:											//比倍
			_bibeiLogic->sendBibeiBtnMessage();
			stopWait();
			_bibeiLogic->handleBiBeiAgain();
			break;
		case 245:											//得分
			_bibeiLogic->handleSecond();
			break;
		default:
			break;
		}

	}

	void GameBiBei::stopWait()
	{
		//移除wait动画
		auto animate = dynamic_cast<Animate*>(_secondBG->getChildByTag(Anm_WAIT));
		if (animate != nullptr)
		{
			animate->release();
		}
	}

	void GameBiBei::BiBeiOver()
	{
		//停止大小和图片切换
		unschedule(schedule_selector(GameBiBei::setNote));
		//隐藏大小和按钮
		char name[20] = {0};
		for (int i = 0; i < 3; i++)
		{
			sprintf(name, "select_%d", i+1);
			auto btn = dynamic_cast<Button*>(_secondBG->getChildByName(name));
			btn->setVisible(false);
			btn->setEnabled(false);
		}
		//移除wait动画
		auto animate = dynamic_cast<Animate*>(_secondBG->getChildByTag(Anm_WAIT));
		if (animate != nullptr)
		{
			animate->reverse();
		}
	}

	void GameBiBei::startPlay()
	{
		GameAudio::playYaosaizi();
		
		playArmature(ROLL);
		this->runAction(Sequence::create(DelayTime::create(3.75f),
			CallFunc::create(CC_CALLBACK_0(GameBiBei::playgo, this)), nullptr));
	}

	void GameBiBei::playgo()
	{
		playArmature(WAIT);
		Director::getInstance()->getTextureCache()->removeAllTextures();
		//大小和图片切换
		schedule(schedule_selector(GameBiBei::setNote), 0.5);
		//显示大小和按钮
		for (int i = 0; i < 3; i++)
		{
			_btn[i]->setVisible(true);
			_btn[i]->setEnabled(true);
		}
	}

	void GameBiBei::setNote(float delay)
	{
		auto note = dynamic_cast<ImageView*>(_secondBG->getChildByName("Note"));
		if (note->isVisible())
		{
			note->setVisible(false);
		}
		else
		{
			note->setVisible(true);
		}
	}

	void GameBiBei::initBIBEI(int value)
	{
		if (value == 1)
		{
			_myAnimation.name = "roll";
			_myAnimation.count = 24;
			_myAnimation.playCount = 0;
			_myAnimation.playtime = 8.0/60;
			_myAnimation.removeTime = 7.9/60;
		}
		else if (value == 2)
		{
			_myAnimation.name = "wait";
			_myAnimation.count = 7;
			_myAnimation.playCount = 0;
			_myAnimation.playtime = 10.0/60;
			_myAnimation.removeTime = 9.9/60;
		}
		else if (value == 3)
		{
			_myAnimation.name = "win";
		}
		else
		{
			_myAnimation.name = "lose";
		}
	}

	void GameBiBei::playAnimation(float delay)
	{
		if (_myAnimation.playCount > _myAnimation.count)
		{
			if (_myAnimation.name != "wait")
			{
				unschedule(schedule_selector(GameBiBei::playAnimation));
				return;
			}
			else
			{
				initBIBEI(2);
			}
		}
		
		char name[64] = {0};
		sprintf(name, "WaterMargin/BiBei/Boss/%s/%dx.png", _myAnimation.name, _myAnimation.playCount);
		_bossSprite = Sprite::create(name);
		_bossSprite->setPosition(_pos[0]);
		_secondBG->addChild(_bossSprite);
		

		sprintf(name, "WaterMargin/BiBei/Left/%s/aa%dx.png", _myAnimation.name, _myAnimation.playCount);
		_leftSprite = Sprite::create(name);
		_leftSprite->setPosition(_pos[1]);
		_secondBG->addChild(_leftSprite);
		

		sprintf(name, "WaterMargin/BiBei/Right/%s/%dx.png", _myAnimation.name, _myAnimation.playCount);
		_rightSprite = Sprite::create(name);
		_rightSprite->setPosition(_pos[2]);
		_secondBG->addChild(_rightSprite);

		_myAnimation.playCount++;

		this->runAction(Sequence::create(DelayTime::create(_myAnimation.removeTime), CallFunc::create([&]()
		{
			_bossSprite->removeFromParent();
			_leftSprite->removeFromParent();
			_rightSprite->removeFromParent();

			_bossSprite = nullptr;
			_leftSprite = nullptr;
			_rightSprite = nullptr;
		}), nullptr));

	}

	void GameBiBei::playArmature(BYTE aniType)
	{
		_aniB->stopAllActions();
		_aniL->stopAllActions();
		_aniR->stopAllActions();
		float time = 0.0;
		BYTE typeR, typeL, typeB;
		switch (aniType)
		{
		case 0://等待
			time = 1.5f;
			typeR = 1; 
			typeL = 1;
			typeB = 3;
			_aniB->setPosition(_pos_bosswait);
			_aniL->setPosition(_pos_leftwait);
			_aniR->setPosition(_pos_rightwait);
			break;
		case 1://输
			time = 150.0f / 60.0f;
			typeR = 0;
			typeL = 0;
			typeB = 0;
			_aniB->setPosition(_pos_bosslost);
			_aniL->setPosition(_pos_leftlost);
			_aniR->setPosition(_pos_rightlost);
			break;
		case 2://赢
			time = 111.0f / 60.0f;
			typeR = 2;
			typeL = 2;
			typeB = 4;
			_aniB->setPosition(_pos_bosswin);
			_aniL->setPosition(_pos_leftwin);
			_aniR->setPosition(_pos_rightwin);
			break;
		case 3://摇色子
			time = 90.0f / 60.0f;
			typeR = 2;
			typeL = 2;
			typeB = 2;
			_aniB->setPosition(_pos_bossthrow);
			_aniL->setPosition(_pos_leftwin);
			_aniR->setPosition(_pos_rightwin);
			break;
		default:
			time = -1;
			break;
		}

		auto animateR = _gameAni->createBiBeiAni(3, typeR);
		auto animateL = _gameAni->createBiBeiAni(4, typeL);
		auto animateB = _gameAni->createBiBeiAni(5, typeB);
		auto animateOpen = _gameAni->createBiBeiAni(5, 1);

		//BOSS
		if (aniType == 1)
		{
			_aniB->runAction(Sequence::create(CallFunc::create([=](){
				_aniB->setPosition(_pos_bossopen);
			}), animateOpen, CallFunc::create([=](){
				_aniB->setPosition(_pos_bosslost);
			}), animateB, nullptr));
		}
		else if (aniType == 2)
		{
			_aniB->runAction(Sequence::create(CallFunc::create([=](){
				_aniB->setPosition(_pos_bossopen);
			}), animateOpen, CallFunc::create([=](){
				_aniB->setPosition(_pos_bosswin);
			}), animateB, nullptr));
		}
		else
		{
			_aniB->runAction(animateB);
		}
		_aniR->runAction(animateR);
		_aniL->runAction(animateL);

	}
}
