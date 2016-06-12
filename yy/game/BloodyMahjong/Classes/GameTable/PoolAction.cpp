#include "PoolAction.h"

namespace XZMJ
{

	PoolAction::PoolAction(void)
	{
		_runSp = nullptr;
	}

	PoolAction::~PoolAction(void)
	{
		_vSpFrame.clear();
	}

	void PoolAction::setInfo(CardPool* pool, const INT& cardNumber)
	{
		_cardNumber = cardNumber;
		_pool = pool;
		run();
	}

	void PoolAction::uiAction(std::string name)
	{
		auto animation = getAni(name);
		assert(animation !=  nullptr);
		if (!_runSp->isVisible())
		{
			_runSp->setPosition(_pool->getActionPos());
			_runSp->runAction(
				Sequence::create(
				CallFunc::create(CC_CALLBACK_0(Node::setVisible, _runSp, true)),
				Animate::create(animation),
				CallFunc::create(CC_CALLBACK_0(Node::setVisible, _runSp, false)),
				nullptr));
		}
		else
		{
			auto sp = Sprite::createWithSpriteFrameName("huAction1.png");
			this->addChild(sp);
			sp->setVisible(false);
			sp->setPosition(_pool->getActionPos());
			sp->runAction(
				Sequence::create(
				CallFunc::create(CC_CALLBACK_0(Node::setVisible, _runSp, true)),
				Animate::create(animation),
				CallFunc::create(CC_CALLBACK_0(Node::setVisible, _runSp, false)),
				CallFunc::create(CC_CALLBACK_0(Node::removeFromParent, sp)),
				nullptr));
		}
	}
	
	Animation* PoolAction::getAni(std::string name)
	{
		auto cache = AnimationCache::getInstance();
		Animation* animation = nullptr;
		animation = cache->getAnimation(name);
		if (animation == nullptr)
		{
			for (auto i = 1; i < 50; i++)
			{
				auto fullName = StringUtils::format("%s%d.png", name.c_str(), i);
				auto spf = SpriteFrameCache::getInstance()->getSpriteFrameByName(fullName.c_str());
				if (spf != nullptr)
				{
					_vSpFrame.pushBack(spf);
				}
				else
				{
					break;
				}
			}
			animation = Animation::createWithSpriteFrames(_vSpFrame);
			animation->setDelayPerUnit(0.05f);
			animation->setLoops(1);
			cache->addAnimation(animation, name);
		}
		return animation;
	}

}