#ifndef __HN_HNLayerColor_H__
#define __HN_HNLayerColor_H__

#include "cocos2d.h"

USING_NS_CC;

namespace HN {

	class HNLayerColor : public LayerColor
	{
	public:
		HNLayerColor(void);
		virtual ~HNLayerColor(void);

		virtual bool init() override;
	};
};

#endif	//__HN_HNLayerColor_H__
