#include "HNSprite.h"

namespace HN 
{

	HNSprite* HNSprite::create(const std::string& filename)
	{
		HNSprite *sprite = new (std::nothrow) HNSprite();
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	HNSprite::HNSprite(void)
	{

	}

	HNSprite::~HNSprite(void)
	{

	}

}