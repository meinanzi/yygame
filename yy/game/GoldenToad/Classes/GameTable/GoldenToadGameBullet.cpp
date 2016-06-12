#include "GoldenToadGameBullet.h"
#include "GoldenToadGamePlayer.h"
#include "GoldenToadGameMathAide.h"
#include "GoldenToadGameAudio.h"

namespace GoldenToad
{
	GameBullet* GameBullet::create(const std::string & file, GamePlayer* player, GameFishManager* fishManager, BulletKind bulletKind, INT bulletId, INT bulletMulriple, FLOAT net_radius, FLOAT speed)
	{
		GameBullet* pRet = new GameBullet(player, fishManager, bulletKind, bulletId, bulletMulriple, net_radius, speed);
		if (nullptr != pRet && pRet->initWithSpriteFrameName(file))
		{
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}

	GameBullet::GameBullet(GamePlayer* player, GameFishManager* fishManager, BulletKind bulletKind, INT bulletId, INT bulletMulriple, FLOAT net_radius, FLOAT speed):
		_player(player), _fishManager (fishManager), _bullet_kind( bulletKind), 
		_bullet_id(bulletId), _bullet_mulriple(bulletMulriple), _netRadius(net_radius), 
		_speed(speed), onHitFishCallBack (nullptr), _isDoUpdateFUNC (true)
	{
		_lockFishId = INVALID_FISHID;
	}

	GameBullet::~GameBullet()
	{
	}

	void GameBullet::runStartAction(float radians)
	{
		_radians = radians;
		_WinSize = Director::getInstance()->getWinSize();

		scheduleUpdate();
	}

	void GameBullet::collisionDetection()
	{
		auto rect = this->getBoundingBox();
		auto currentSize = Size(rect.size.width / 2, rect.size.height / 2);
		rect.size = currentSize;

		for (auto ite = _fishManager->_fishList.begin(); ite != _fishManager->_fishList.end(); ite++)
		{
			auto fish = (Fish*)*ite;
			if (!fish->isVisible() || !fish->getCanTouch()) continue;

			Fish* touchFish = nullptr;
			//如果有锁定的鱼，必须是鱼id跟锁定id相同才检测碰撞
			if (_lockFishId != INVALID_FISHID)
			{
				if (fish->getFishID() == _lockFishId)
				{
					touchFish = fish;
				}
			}
			else
			{
				touchFish = fish;
			}

			if (!touchFish) continue;

			Rect rt = touchFish->getFishRect();
			if (rt.intersectsRect(rect)) //子弹和鱼碰撞
			{
				//子弹停止前进
				unscheduleUpdate();
				//子弹爆炸
				bulletExplosion();
				int anskind = (int)_bullet_kind;
				if (onHitFishCallBack)
				{
					auto meteor = ParticleSystemQuad::create("goldenToad/particle/dazhongyu.plist");
					meteor->setPosition(this->getPosition());
					meteor->setAutoRemoveOnFinish(true);
					this->getParent()->addChild(meteor, 100);

					//向上一级报告 子弹和鱼 产生碰撞
					onHitFishCallBack(touchFish->getFishID(), _bullet_id, _bullet_kind, _bullet_mulriple);
				}
				break;
			}
		}
	}

	void GameBullet::bulletExplosion()
	{
		this->initWithSpriteFrameName("net0_0.png");

		auto cache = SpriteFrameCache::getInstance();
		Animation *animation = Animation::create();
		animation->setDelayPerUnit(0.08f);
		animation->setLoops(1);

		for (BYTE i = 0; i <= 6; i++)
		{
			String name = StringUtils::format("net%d_%d.png", _bullet_kind, i);
			animation->addSpriteFrame(cache->getSpriteFrameByName(name.getCString()));
		}
		//this->setScale(0.7f);
		auto animate = Animate::create(animation);
		this->runAction(Sequence::create(animate, CallFunc::create([this](){

			for (auto ite = _player->_bulletList.begin(); ite != _player->_bulletList.end(); ite++)
			{
				if (*ite == this)
				{
					_player->_bulletList.erase(ite);
					break;
				}
			}

			removeFromParent();
		}), nullptr));
	}

	void GameBullet::update(float dt)
	{
		if (!_isDoUpdateFUNC && _lockFishId == INVALID_FISHID)
		{
			_isDoUpdateFUNC = true;
			return;
		}
		_isDoUpdateFUNC = false;

		//检测与鱼边界碰撞
		collisionDetection();
		auto currentPos = this->getPosition();
		//检测与屏幕左右边界碰撞
		if (currentPos.x <= 0 || currentPos.x >= _WinSize.width)
		{
			setRotation(-getRotation());
		}
		//检测与屏幕上下边界碰撞
		if (currentPos.y <= 0 || currentPos.y >= _WinSize.height)
		{
			setRotation(180 - getRotation());
		}

		if (_lockFishId == INVALID_FISHID)
		{
			//运动弧度
			_radians = CC_DEGREES_TO_RADIANS(90 - getRotation());			
		}
		else
		{
			//锁定状态子弹不再按照出枪管时的路线飞行，而是根据锁定的鱼的位置跟踪移动
			auto fish = dynamic_cast<Fish*>(_fishManager->getFishById(_lockFishId));
			if (nullptr != fish)
			{
				Vec2 fishPos = fish->getPosition();
				//目标鱼和子弹直接的距离
				float range = sqrt(pow(abs(fishPos.x - currentPos.x), 2) + pow(abs(fishPos.y - currentPos.y), 2));
				//目标鱼和子弹的弧度
				_radians = atan2(fishPos.y - currentPos.y, fishPos.x - currentPos.x);
			}
		}

		//利用已知的弧度和位移速度获得下一个坐标
		currentPos.x = getPositionX() + cosf(_radians) * _speed;
		currentPos.y = getPositionY() + sinf(_radians) * _speed;

		setPosition(currentPos);
	}
}