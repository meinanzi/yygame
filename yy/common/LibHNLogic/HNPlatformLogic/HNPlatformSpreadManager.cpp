//Copyright (c) 2012¡ª2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved

#include "HNPlatformSpreadManager.h"


namespace HN
{
	HNSpreadManager* HNSpreadManager::_spreadManager = nullptr;

	HNSpreadManager* HNSpreadManager::getInstance()
	{
		if (nullptr == _spreadManager)
		{
			_spreadManager = new HNSpreadManager();
		}
		return _spreadManager;
	}

	HNSpreadManager::HNSpreadManager()
	{
	}

	HNSpreadManager::~HNSpreadManager()
	{
	}
}
