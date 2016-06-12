#include "HNDialogBase.h"

USING_NS_CC;

namespace HN
{
	void HNDialogBase::show()
	{
		if(_parent != nullptr)
		{
			this->setPosition(_hidePoint);
			this->setScale(0);

			auto spawn = Spawn::create(ScaleTo::create(0.2f, 0.8f), EaseIn::create(MoveTo::create(0.2f, _showPoint), 1.5f), nullptr);
			this->runAction(spawn);
		}
	}

	void HNDialogBase::hide()
	{
		if(_parent)
		{
			auto sequence = Sequence::create(
				Spawn::create(ScaleTo::create(0.2f,0), EaseOut::create(MoveTo::create(0.2f, _hidePoint), 1.0f), nullptr), 
				RemoveSelf::create(true), nullptr);
			this->runAction(sequence);
		}
	}

	bool HNDialogBase::init(cocos2d::Node* parent)
	{
		if(!HNLayer::init())
		{
			return false;
		}
		_parent = parent;
		_parent->addChild(this);
		this->ignoreAnchorPointForPosition(false);
		this->setAnchorPoint(Vec2(0.5f,0.5f));
		return true;
	}

	HNDialogBase::HNDialogBase():_parent(nullptr)
	{
		Size size = Director::getInstance()->getVisibleSize();
		_showPoint.setPoint(size.width / 2, size.height / 2);
		_hidePoint.setPoint(size.width / 2, size.height / 2);
	}

	HNDialogBase::~HNDialogBase()
	{

	}

	void HNDialogBase::locateMid(cocos2d::Node* pNode)
	{
		Size size = Director::getInstance()->getVisibleSize();
		pNode->ignoreAnchorPointForPosition(false);
		pNode->setAnchorPoint(Vec2(0.5f,0.5f));
		pNode->setPosition(size.width / 2, size.height / 2);
	}
}