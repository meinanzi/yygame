#ifndef __HN_HNSprite_H__
#define __HN_HNSprite_H__

#include "cocos2d.h"

USING_NS_CC;

namespace HN {

	class HNSprite : public Sprite
	{
	public:
		HNSprite(void);
		static HNSprite* create(const std::string& filename);
		virtual ~HNSprite(void);
	};

};

#endif	//__HN_HNSprite_H__

