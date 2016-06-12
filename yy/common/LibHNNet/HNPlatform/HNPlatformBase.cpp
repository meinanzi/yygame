#include "HNPlatformBase.h"
#include "HNPlatform/HNPlatformLogic.h"

namespace HN 
{

	HNPlatformBase::HNPlatformBase(void)
	{
	}

	HNPlatformBase::~HNPlatformBase(void)
	{
	}

	void HNPlatformBase::onEnter()
	{
		HNScene::onEnter();
		PlatformLogic()->addObserver(this);
	}

	void HNPlatformBase::onExit()
	{
		HNScene::onExit();
		PlatformLogic()->removeObserver(this);
	}
}