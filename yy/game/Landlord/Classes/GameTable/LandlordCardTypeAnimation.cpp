#include "LandlordCardTypeAnimation.h"
#include "LandlordGameAudio.h"

USING_NS_CC;

namespace landlord
{
	#define WIN_SIZE  Director::getInstance()->getWinSize()

	//////////////////////////////////////////////////////////////////////////
	static const char * rocket_anim_png	= "landlord/game/card_type/rocket.png";
	static const char * rocket_anim_png_format	= "landlord/game/card_type/rocket_fire%d.png";
	
	static const char * bobm_png = "landlord/game/card_type/missle.png";
	static const char * bobm_png_format = "landlord/game/card_type/missle_bomb%d.png";

	static const char * straight_png = "landlord/game/card_type/straight.png";
	static const char * straight_boat_png = "landlord/game/card_type/straight_boat.png";
	static const char * straight_wind_format = "landlord/game/card_type/straight_wind%d.png";

	static const char * double_line_png = "landlord/game/card_type/double_line.png";
	static const char * double_yuanyang_png = "landlord/game/card_type/double_yuanyang.png";
	static const char * double_water_png_format = "landlord/game/card_type/double_water%d.png";

	static const char * plane_png = "landlord/game/card_type/plane.png";

	//////////////////////////////////////////////////////////////////////////

	void CardTypeAnimation::playRocket(cocos2d::Node* parent, int zorder)
	{
		Vec2 offset(0, 80.0f);
		auto rocket = Sprite::create(rocket_anim_png);
		parent->addChild(rocket, zorder);
		rocket->setPosition(WIN_SIZE.width / 2 + offset.x, WIN_SIZE.height / 2 - offset.y);
		rocket->runAction(Sequence::create(MoveBy::create(2.0f, offset * 2), FadeOut::create(0.5f), RemoveSelf::create(), nullptr));

		char str[128] = {0};
		auto fire = Sprite::create();
		auto animation = Animation::create();
		for(int i = 0 ;i < 2; i++)
		{
			sprintf(str, rocket_anim_png_format, i + 1);
			animation->addSpriteFrameWithFile(str);
		}
		animation->setDelayPerUnit(1.0f);
		fire->runAction(RepeatForever::create(Animate::create(animation)));
		fire->setAnchorPoint(Vec2(0.5f, 1));
		rocket->addChild(fire);
		fire->setPosition(rocket->getContentSize().width / 2, 90);

		GameAudio::playRocketEffect();
	}

	void CardTypeAnimation::playBomb(cocos2d::Node* parent, int zorder)
	{
		Vec2 offset(80.0f, 80.0f);
		auto bomb = Sprite::create(bobm_png);
		parent->addChild(bomb, zorder);
		bomb->setPosition(WIN_SIZE.width / 2 + offset.x, WIN_SIZE.height / 2 + offset.y);
		bomb->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(- offset.x * 2, -offset.y * 2)), FadeOut::create(0.5f), RemoveSelf::create(), nullptr));

		auto animation = Animation::create();
		for(int i = 0; i < 5; i++)
		{
			char str[50] = {0};
			sprintf(str, bobm_png_format, i + 1);
			animation->addSpriteFrameWithFile(str);
		}
		animation->setDelayPerUnit(1.5f / 5);
		auto power = Sprite::create();
		parent->addChild(power, zorder);
		power->setPosition(WIN_SIZE.width / 2 - offset.x + 47, WIN_SIZE.height / 2 - offset.y + 24);
		power->runAction(Sequence::create(DelayTime::create(0.5f),
			//CallFunc::create(GameAudio::playBombEffect),
			//DelayTime::create(1.0f),
			Animate::create(animation),
			RemoveSelf::create(), nullptr));

		GameAudio::playBombEffect();
	}

	void CardTypeAnimation::playStraight(cocos2d::Node* parent, int zorder)
	{
		auto sp = Sprite::create(straight_png);
		parent->addChild(sp, zorder);
		sp->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
		sp->runAction(Sequence::create(DelayTime::create(1.5f), RemoveSelf::create(), nullptr));

		Vec2 offset(80.0f, 0);
		auto boat = Sprite::create(straight_boat_png);
		parent->addChild(boat, zorder);
		boat->setPosition(WIN_SIZE.width / 2 - offset.x, WIN_SIZE.height / 2 - offset.y);
		boat->runAction(Sequence::create(MoveBy::create(1.5f, Vec2(offset.x * 2, offset.y * 2)), FadeOut::create(0.5f), RemoveSelf::create(), nullptr));

		auto animation = Animation::create();
		for(int i = 0; i < 7; i++)
		{
			char str[50] = {0};
			sprintf(str, straight_wind_format, i + 1);
			animation->addSpriteFrameWithFile(str);
		}
		animation->setDelayPerUnit(1.5f / 7);
		auto water = Sprite::create();
		boat->addChild(water);
		water->setPosition(boat->getContentSize().width / 2, 0);
		water->runAction(Sequence::create(Animate::create(animation), RemoveSelf::create(), nullptr));

		GameAudio::playStraightEffect();
	}

	void CardTypeAnimation::playDoubleStraight(cocos2d::Node* parent, int zorder)
	{
		auto sp = Sprite::create(double_line_png);
		parent->addChild(sp, zorder);
		sp->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
		sp->runAction(Sequence::create(DelayTime::create(1.0f), RemoveSelf::create(), nullptr));

		Vec2 offset(80.0f, 0);
		auto bird = Sprite::create(double_yuanyang_png);
		parent->addChild(bird, zorder);
		bird->setPosition(WIN_SIZE.width / 2 + offset.x, WIN_SIZE.height / 2 + offset.y);
		bird->runAction(Sequence::create(MoveBy::create(1.0f, Vec2(- offset.x * 2, -offset.y * 2)), RemoveSelf::create(), nullptr));
	
		char str[128] = {0};
		auto animation = Animation::create();
		for(int i = 0; i < 2; i++)
		{
			sprintf(str, double_water_png_format, i + 1);
			animation->addSpriteFrameWithFile(str);
		}
		animation->setDelayPerUnit(0.1f);
		auto water = Sprite::create();
		bird->addChild(water);
		water->setPosition(bird->getContentSize().width / 2, 0);
		water->runAction(Sequence::create(Repeat::create(Animate::create(animation), 10), RemoveSelf::create(), nullptr));
		
		GameAudio::playDoubleSequenceEffect();
	}

	void CardTypeAnimation::playThree(cocos2d::Node* parent, int zorder)
	{
		Vec2 offset(80.0f, 0);
		auto plan = Sprite::create(plane_png);
		parent->addChild(plan, zorder);
		plan->setPosition(WIN_SIZE.width / 2 + offset.x, WIN_SIZE.height / 2 + offset.y);
		plan->runAction(Sequence::create(MoveBy::create(1.5f, Vec2(- offset.x * 2, -offset.y * 2)), FadeOut::create(0.5f), RemoveSelf::create(), nullptr));
		
		GameAudio::playPlanEffect();
	}

}


