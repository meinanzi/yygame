#include "GoldenToadGamePlayer.h"
#include "GoldenToadGameBullet.h"
#include "GoldenToadGameTableUI.h"
#include "GoldenToadGameUserMessageBox.h"
#include "GoldenToadGameAudio.h"

namespace GoldenToad
{
	GamePlayer::GamePlayer()
		: _AtlasLabel_Money (nullptr)
		, _AtlasLabel_CannonNum (nullptr)
		, _Layout_Table (nullptr)
		, _Image_Battery (nullptr)
		, _Image_MoneyBox (nullptr)
		, _Sprite_Cannon (nullptr)
		, _Image_Card (nullptr)
		, _Image_Lock (nullptr)
		, _Sprite_Bingo (nullptr)
		, _gumPos(Vec2::ZERO)
		, _cannonKind(BULLET_KIND_1_NORMAL)
		, _bEnergyCannon (false)
		, _currentBulletKind (BULLET_KIND_1_NORMAL)
		, _isMe (false)
		, _isRobot (false)
		, _seatNo (-1)
		, _bLock (false)
		, _tableUI (nullptr)
		, _bulletNum (0)
		, _lockFishId (INVALID_FISHID)
	{
		memset(_bulletSpeed, 0x0, sizeof(_bulletSpeed));
		memset(_netRadius, 0x0, sizeof(_netRadius));
		char str[24];
		for (int i = 0; i<BULLET_KIND_COUNT; i++)
		{
			sprintf(str, "bullet%d.png", i);
			bulletName[i] = str;
		}
	}
	GamePlayer::~GamePlayer()
	{

	}

	GamePlayer* GamePlayer::createGamePlayer(BYTE seatNo, Layout* Panel_Table, GameFishManager* fishManager)
	{
		GamePlayer *player = new GamePlayer();
		if (player && player->init(seatNo, Panel_Table, fishManager))
		{ 
			player->autorelease();
			return player;
		} 
		CC_SAFE_DELETE(player);
		return nullptr;
	}

	bool GamePlayer::init(BYTE seatNo, Layout* Panel_Table, GameFishManager* fishManager)
	{
		if (!HNLayer::init()) return false;

		_seatNo = seatNo;
		_fishManager = fishManager;
		_Layout_Table = Panel_Table;
		char str[24];
		sprintf(str, "Image_Battery%d", seatNo);
		_Image_Battery			= dynamic_cast<ImageView*>(_Layout_Table->getChildByName(str));
		auto winSize = Director::getInstance()->getWinSize();
		float scY = 720 / winSize.height * 0.75f;
		_Image_Battery->setScaleY(scY);

		auto Image_Board		= dynamic_cast<ImageView*>(_Image_Battery->getChildByName("Image_Board"));
		Image_Board->setLocalZOrder(5);

		_AtlasLabel_CannonNum	= dynamic_cast<TextAtlas*>(_Image_Battery->getChildByName("AtlasLabel_CannonNum"));
		_AtlasLabel_CannonNum->setLocalZOrder(6);
	
		_Image_MoneyBox          = dynamic_cast<ImageView*>(_Image_Battery->getChildByName("Image_MoneyBox"));
		_Image_MoneyBox->addClickEventListener([=](Ref* ref){
			Vec2 pos = _Image_Battery->convertToWorldSpace(_Image_MoneyBox->getPosition());

			auto message = GoldenToadUserMessageBox::createMessageBox(_tableUI, _userId, _currentMoney);
			message->setPosition(Vec2(_tableUI->getContentSize().width / 2, _tableUI->getContentSize().height / 2));
			message->setHidePoint(pos);
			_tableUI->reorderChild(message, MAX_ZORDER);
		});

		_AtlasLabel_Money		= dynamic_cast<TextAtlas*>(_Image_MoneyBox->getChildByName("AtlasLabel_Money"));

		_Sprite_Cannon           = dynamic_cast<Sprite*>(_Image_Battery->getChildByName("Sprite_Cannon"));
		_gumPos = _Sprite_Cannon->getPosition();

		_Image_Card				= dynamic_cast<ImageView*>(_Image_Battery->getChildByName("Image_Card"));
		_Image_Card->setCascadeOpacityEnabled(true);
		_Image_Lock				= dynamic_cast<ImageView*>(_Image_Battery->getChildByName("Image_Lock"));
		_Image_Lock->setLocalZOrder(1);
		_Image_Lock->setCascadeOpacityEnabled(true);

		_Sprite_Bingo			= dynamic_cast<Sprite*>(_Image_Battery->getChildByName("Sprite_Bingo"));

		_Button_Add = dynamic_cast<Button*>(_Image_Battery->getChildByName("Button_Add"));

		if (nullptr != _Button_Add)
		{
			_Button_Add->addTouchEventListener(CC_CALLBACK_2(GamePlayer::menuClickCallback, this));
		}

		_Button_Cut = dynamic_cast<Button*>(_Image_Battery->getChildByName("Button_Cut"));

		if (nullptr != _Button_Cut)
		{
			_Button_Cut->addTouchEventListener(CC_CALLBACK_2(GamePlayer::menuClickCallback, this));
		}

		return true;
	}

	void GamePlayer::menuClickCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype)
	{
		if (Widget::TouchEventType::ENDED != touchtype)	return;
		Button* pMenu = dynamic_cast<Button*>(pSender);

		GoldenToadGameAudio::playAddEffect();

		std::string name = pMenu->getName();

		int firepower = _currentFirepower;

		if (name.compare("Button_Add") == 0)
		{
			if (firepower == _max_bullet_multiple)
			{
				//当前火力已经最大了，但是还想加火力，把火力设置最小
				firepower = _min_bullet_multiple;
			}
			else if (firepower < 100)
			{
				firepower += 10;
			}
			else if (firepower >= 100 && firepower < 1000)
			{
				firepower += 100;
			}
			else
			{
				firepower += 1000;
			}

			if (firepower >= _max_bullet_multiple)
			{
				firepower = _max_bullet_multiple;
			}

			setCannonNum(firepower);
		}

		if (name.compare("Button_Cut") == 0)
		{
			if (firepower == _min_bullet_multiple)
			{
				//当前火力已经最小了，但是还想减火力，把火力设置最大
				firepower = _max_bullet_multiple;
			}
			else if (firepower <= 100)
			{
				firepower -= 10;
			}
			else if (firepower > 100 && firepower <= 1000)
			{
				firepower -= 100;
			}
			else
			{
				firepower -= 1000;
			}

			if (firepower <= _min_bullet_multiple)
			{
				firepower = _min_bullet_multiple;
			}

			setCannonNum(firepower);
		}
	}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////
	void GamePlayer::setBatteryVisible(bool bVisible)
	{
		if (nullptr != _Image_Battery)
		{
			_Image_Battery->setVisible(bVisible);
			if (bVisible && _isMe)
			{
				_Button_Add->setVisible(true);
				_Button_Cut->setVisible(true);
			}	
			else
			{
				_Button_Add->setVisible(false);
				_Button_Cut->setVisible(false);
			}
		}
	}

	void GamePlayer::setUserMoney(const LLONG money)
	{
		_currentMoney = money;
		if (nullptr != _AtlasLabel_Money)
		{
			char str[32];
			sprintf(str, "%lld", money);
			_AtlasLabel_Money->setString(str);
		}				
	}

	void GamePlayer::setConfig(CMD_S_GameConfig* gConfig)
	{
		_max_bullet_multiple = gConfig->max_bullet_multiple;
		_min_bullet_multiple = gConfig->min_bullet_multiple;

		for (BYTE i = 0; i < BULLET_KIND_COUNT; i++)
		{
			_bulletSpeed[i] = gConfig->bullet_speed[i];
			_netRadius[i] =  gConfig->net_radius[i];
		}
	}

	void GamePlayer::setCannonNum(const INT cannonNum)
	{
		if (_currentFirepower == cannonNum) return;
		_currentFirepower = cannonNum;

		if (nullptr != _AtlasLabel_CannonNum)
		{
			char str[24];
			sprintf(str, "%d", cannonNum);
			_AtlasLabel_CannonNum->setString(str);
		}

		if (_isMe)
		{
			float minMultiple = _max_bullet_multiple * 0.25f;
			float midMultiple = _max_bullet_multiple * 0.5f;
			float bigMultiple = _max_bullet_multiple * 0.75f;

			if (cannonNum <= minMultiple)
			{
				if (_bEnergyCannon)
				{
					_currentBulletKind = BULLET_KIND_1_ION;
				}
				else
				{
					_currentBulletKind = BULLET_KIND_1_NORMAL;
				}
			}
			else if (cannonNum > minMultiple && cannonNum <= midMultiple)
			{
				if (_bEnergyCannon)
				{
					_currentBulletKind = BULLET_KIND_2_ION;
				}
				else
				{
					_currentBulletKind = BULLET_KIND_2_NORMAL;
				}
			}
			else if (cannonNum > midMultiple && cannonNum <= bigMultiple)
			{
				if (_bEnergyCannon)
				{
					_currentBulletKind = BULLET_KIND_3_ION;
				}
				else
				{
					_currentBulletKind = BULLET_KIND_3_NORMAL;
				}
			}
			else
			{
				if (_bEnergyCannon)
				{
					_currentBulletKind = BULLET_KIND_4_ION;
				}
				else
				{
					_currentBulletKind = BULLET_KIND_4_NORMAL;
				}
			}

			if (_max_bullet_multiple == _min_bullet_multiple)
			{
				if (_bEnergyCannon)
				{
					_currentBulletKind = BULLET_KIND_4_ION;
				}
				else
				{
					_currentBulletKind = BULLET_KIND_4_NORMAL;
				}
			}

			setCannonType(_currentBulletKind);
		}
	}

	//设置鱼炮类型
	void GamePlayer::setCannonType(BulletKind kind)
	{			
		if (_cannonKind == kind) return;
		_cannonKind = kind;

		char name[32];
		sprintf(name, "cannon_%d.png", kind);
		_Sprite_Cannon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
	}

	//设置是否能量炮
	void GamePlayer::setIsEnergyCannon(bool isEnergyCannon)
	{
		_bEnergyCannon = isEnergyCannon;

		if (_bEnergyCannon)
		{
			if (!_Image_Card->isVisible())
			{
				_Image_Card->setVisible(true);
				_Image_Card->setOpacity(0);
				auto fadeIn = FadeIn::create(1.5f);
				_Image_Card->runAction(RepeatForever::create(Sequence::create(fadeIn, DelayTime::create(1.0f), fadeIn->reverse(), nullptr)));
			}
			if (_isMe)
			{
				int num = _currentBulletKind;
				if (_currentBulletKind < 4)
				{
					num += 4;
					_currentBulletKind = BulletKind(num);
				}
			}
		}
		else
		{
			_Image_Card->stopAllActions();
			_Image_Card->setVisible(false);

			if (_isMe)
			{
				int num = _currentBulletKind;
				if (_currentBulletKind >= 4)
				{
					num -= 4;
					_currentBulletKind = BulletKind(num);
				}
			}
		}
	}

	void GamePlayer::setIsLockFish(bool bLock, INT fishId)
	{
		if (bLock)
		{
			_bLock = bLock;
			auto lockFish = _fishManager->getFishById(fishId);
			lockFish->setFishIsLock(true);

			schedule(schedule_selector(GamePlayer::showLockIndicatorLine), 0.02f);

			if (!_Image_Lock->isVisible())
			{
				_Image_Lock->setVisible(true);
				_Image_Lock->setOpacity(0);
				auto fadeIn = FadeIn::create(1.5f);
				_Image_Lock->runAction(RepeatForever::create(Sequence::create(fadeIn, DelayTime::create(1.0f), fadeIn->reverse(), nullptr)));
			}

			if (_lockFishId != fishId)
			{
				_lockFishId = fishId;
				_Image_Lock->removeAllChildren();

				auto lockFish = _fishManager->getFishById(fishId);
				auto fish = Fish::create(0, lockFish->getFishKind(), lockFish->_fishKind_ex);
				fish->setPosition(Vec2(_Image_Lock->getContentSize().width / 2, _Image_Lock->getContentSize().height * 0.4f));
				if (lockFish->getFishKind() < FISH_KIND_9 || (lockFish->getFishKind() == FISH_KIND_31 && lockFish->getFishKindEx() < FISH_KIND_9))
				{
					fish->setScale(1.0f);
				}
				if (lockFish->getFishKind() >= FISH_KIND_9 &&  lockFish->getFishKind() < FISH_KIND_19 
					|| (lockFish->getFishKind() == FISH_KIND_31 && lockFish->getFishKindEx() >= FISH_KIND_9
					|| lockFish->getFishKind() == FISH_KIND_32))
				{
					fish->setScale(0.6f);
				}
				else if ((lockFish->getFishKind() >= FISH_KIND_19 && lockFish->getFishKind() < FISH_KIND_23)
					|| (lockFish->getFishKind() >= FISH_KIND_27 && lockFish->getFishKind() <= FISH_KIND_30))
				{
					fish->setScale(0.3f);
				}
				else if ((lockFish->getFishKind() >= FISH_KIND_23 && lockFish->getFishKind() <= FISH_KIND_26)
					|| lockFish->getFishKind() == FISH_KIND_32)
				{
					fish->setScale(0.5f);
				}
				else
				{

				}
				
				fish->setRotation(-90);
				_Image_Lock->addChild(fish);
			}			
		}
		else
		{
			if (_lockFishId == fishId)
			{
				_bLock = bLock;
				_lockFishId = INVALID_FISHID;
				for (auto bullet : _bulletList)
				{
					bullet->setLockFish_id(INVALID_FISHID);
				}
				_bulletList.clear();
				_Image_Lock->setVisible(false);
				_Image_Lock->removeAllChildren();
				_Image_Lock->stopAllActions();
			}
		}		
	}

	//锁定指示气泡线条
	void GamePlayer::showLockIndicatorLine(float dt)
	{
		auto cache = SpriteFrameCache::getInstance();
		if (_lockFishId != INVALID_FISHID)
		{
			auto fish = dynamic_cast<Fish*>(_fishManager->getFishById(_lockFishId));
			if (nullptr != fish)
			{
				auto oldTip = dynamic_cast<Sprite*>(_tableUI->getChildByName("bubbleTip"));
				if (nullptr != oldTip)
				{
					oldTip->removeFromParent();
				}
				for (auto bubble : _bubbleList)
				{
					bubble->removeFromParent();
				}
				_bubbleList.clear();

				auto fishPos = fish->getPosition();
				auto cannonPos = getCannonPoint();

				char str[24];
				sprintf(str, "lock_flag_%d.png", _seatNo + 1);
				auto tip = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(str));
				tip->setPosition(fishPos);
				tip->setName("bubbleTip");
				_tableUI->addChild(tip, FISH_ZORDER + 2);

				float range = sqrt(pow(abs(fishPos.x - cannonPos.x), 2) + pow(abs(fishPos.y - cannonPos.y), 2));

				float radians = atan2(fishPos.y - cannonPos.y, fishPos.x - cannonPos.x);
				float degree = 270 - CC_RADIANS_TO_DEGREES(radians);
				if (_seatNo >= 3)
				{
					degree -= 180;
				}

				_Sprite_Cannon->setRotation(degree);

				int num = (int)(range / 70);

				for (int i = 1, k = 0; i <= num; i++, k++)
				{
					auto bubble = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName("lock_line.png"));

					float temp = (float)k / (num + 5);
					float scaly = 0.5f + temp;
					bubble->setScale(scaly);

					float len = 70 * i;
					float posx = cannonPos.x + len * cos(radians);
					float posy = cannonPos.y + len * sin(radians);

					bubble->setPosition(Vec2(posx, posy));
					_tableUI->addChild(bubble, FISH_ZORDER + 2);
					_bubbleList.push_back(bubble);

					if (_seatNo == 0)
					{
						bubble->setColor(Color3B(255, 0, 0));
						tip->setColor(Color3B(255, 0, 0));
					}
					else if (_seatNo == 1)
					{
						bubble->setColor(Color3B(255, 255, 0));
						tip->setColor(Color3B(255, 255, 0));
					}
					else if (_seatNo == 2)
					{
						bubble->setColor(Color3B(0, 255, 0));
						tip->setColor(Color3B(0, 255, 0));
					}
					else if (_seatNo == 3)
					{
						bubble->setColor(Color3B(0, 0, 255));
						tip->setColor(Color3B(0, 0, 255));
					}
					else if (_seatNo == 4)
					{
						bubble->setColor(Color3B(255, 0, 255));
						tip->setColor(Color3B(255, 0, 255));
					}
					else
					{
						bubble->setColor(Color3B(255, 165, 0));
						tip->setColor(Color3B(255, 165, 0));
					}
				}
			}
		}
		else
		{
			unschedule(schedule_selector(GamePlayer::showLockIndicatorLine));

			auto oldTip = dynamic_cast<Sprite*>(_tableUI->getChildByName("bubbleTip"));
			if (nullptr != oldTip)
			{
				oldTip->removeFromParent();
			}
			for (auto bubble : _bubbleList)
			{
				bubble->removeFromParent();
			}
			_bubbleList.clear();
		}
	}

	void GamePlayer::setBingo(LLONG money)
	{
		char str[12];
		auto bingoMoney = dynamic_cast<TextAtlas*>(_Sprite_Bingo->getChildByName("AtlasLabel_BingoMoney"));
		_Sprite_Bingo->setVisible(true);
		_Sprite_Bingo->setOpacity(255);
		sprintf(str, "%lld", money);
		bingoMoney->setString(str);
		bingoMoney->setRotation(-20.0f);
		bingoMoney->stopAllActions();
		auto rotate = RotateBy::create(0.4f, 40.0f);
		bingoMoney->runAction(RepeatForever::create(Sequence::create(rotate, rotate->reverse(), nullptr)));

		Animation *animation = Animation::create();
		animation->setDelayPerUnit(0.2f);
		animation->setLoops(-1);

		GoldenToadGameAudio::playBingoEffect();

		for (BYTE i = 1; i <= 10; i++)
		{
			std::string name("goldenToad/GameTableUi/Res/bingo1_");
			sprintf(str,"%02d.png",i);
			name.append(str);
			animation->addSpriteFrameWithFile(name);
		}
		auto animate = Animate::create(animation);
		_Sprite_Bingo->runAction(animate);

		_Sprite_Bingo->runAction(Sequence::create(DelayTime::create(5.0f), FadeOut::create(1.0f), nullptr));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////

	bool GamePlayer::batteryIsVisible()
	{
		return _Image_Battery->isVisible();
	}

	bool GamePlayer::isFireEnable()
	{
		if (0 == _currentMoney)
		{
			return false;
		}
		return _currentMoney >= _currentFirepower ? true : false;
	}

	Vec2 GamePlayer::getCannonPoint()
	{
		return _Image_Battery->convertToWorldSpace(_Sprite_Cannon->getPosition());
	}

	Vec2 GamePlayer::getMoneyPoint()
	{
		return _Image_Battery->convertToWorldSpace(_AtlasLabel_Money->getPosition());
	}

	FLOAT GamePlayer::getBulletNetRadiusById(BYTE id)
	{
		auto winSize = Director::getInstance()->getWinSize();
		return _netRadius[id] * winSize.width / kResolutionWidth;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////

	void GamePlayer::gameFire(FLOAT degree, BYTE seatNo, INT bulletId, BulletKind bulletKind, INT bulletMulriple, INT lockFishId, bool isRobot)
	{
		GameBullet* bullet = nullptr;
		FLOAT speed = _bulletSpeed[bulletKind];
		//在服务器没有发来配置文件信息时， 所有玩家开枪的子弹都不显示
		//在屏幕子弹数目大于20颗时，不再显示开火
		if (speed < 0.000001f || _bulletNum > 20) return ;

		if (!_isMe)
		{
			//更新玩家的炮火威力
			setCannonNum(bulletMulriple);
		}

		//更新玩家子弹类型
		setBullet_Kind(bulletKind);
		//更新炮管类型
		setCannonType(bulletKind);

		if (_bEnergyCannon) 
			GoldenToadGameAudio::playIonFireEffect();
		else
			GoldenToadGameAudio::playFireEffect();

		char str[24];
		if (_isMe)
		{
			sprintf(str, "bullet_%d_%d.png", 1, bulletKind);
		}
		else sprintf(str, "bullet_%d_%d.png", 0, bulletKind);

		bullet = GameBullet::create(str, this, _fishManager, bulletKind , bulletId , 
			_currentFirepower, getBulletNetRadiusById(bulletKind), speed);

		auto cannon_size = _Sprite_Cannon->getContentSize();
		_Sprite_Cannon->setRotation(degree);
		if (seatNo < 3)
		{
			_Sprite_Cannon->setRotation(degree + 180);
		}	

		playCannonFireAction();
		_tableUI->addChild(bullet , BULLET_ZORDER);

		//如果有锁定鱼，则把之后发射的子弹都加进列表中管理
		if (_bLock) 
		{
			bullet->setLockFish_id(lockFishId);
			_bulletList.push_back(bullet);
		}

		bullet->setAnchorPoint(Vec2(0.5f , 0.5f));
		bullet->setScale(0.8f);
		bullet->setRotation(degree);
		bullet->setPosition(_Sprite_Cannon->convertToWorldSpace(Vec2(cannon_size.width * 0.55f , cannon_size.height * 0.9f)));
		bullet->runStartAction(CC_DEGREES_TO_RADIANS(degree - 90));

		//每开炮一次子弹计数加1
		_bulletNum++;
		//子弹碰撞到鱼计数减1
		bullet->onHitFishCallBack = [=](INT fishId , INT bulletId , BulletKind bulletKind , INT  bulletMuriple){
			_bulletNum--;
			//如果开炮的是机器人，则也需要做碰撞检测，通知服务端
			if (_isMe || isRobot) _tableUI->sendHitFish(seatNo, fishId, bulletId, bulletKind, bulletMuriple);
		};
	}

	//播放鱼炮开火动画
	void GamePlayer::playCannonFireAction()
	{
		_Sprite_Cannon->stopAllActions();
		_Sprite_Cannon->setPosition(_gumPos);

		float degree = _Sprite_Cannon->getRotation();
		float radians = CC_DEGREES_TO_RADIANS(270 - degree);
		float x2 = _gumPos.x + 25 * cosf(radians);
		float y2 = _gumPos.y + 25 * sinf(radians);

		auto move = MoveTo::create(0.04f, Vec2(x2, y2));
		auto moveBack = MoveTo::create(0.04f, _gumPos);
		_Sprite_Cannon->runAction(Sequence::create(move, CallFunc::create([=](){
			auto fire = Sprite::createWithSpriteFrameName("huohua.png");
			fire->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
			fire->setPosition(Vec2(_Sprite_Cannon->getContentSize().width / 2, _Sprite_Cannon->getContentSize().height));
			_Sprite_Cannon->addChild(fire);
			fire->runAction(Sequence::create(DelayTime::create(0.1f), RemoveSelf::create(true), nullptr));
		}), moveBack, nullptr));
	}
}
