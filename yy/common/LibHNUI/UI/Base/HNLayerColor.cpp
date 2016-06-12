#include "HNLayerColor.h"

namespace HN {

	HNLayerColor::HNLayerColor(void)
	{
	}

	HNLayerColor::~HNLayerColor(void)
	{
	}

	bool HNLayerColor::init()
	{
		if (!LayerColor::init())
		{
			return false;
		}
		this->setAnchorPoint(Vec2(0.5f, 0.5f));
		return true;
	}


}