#ifndef __HN_HNPlatformBase_H__
#define __HN_HNPlatformBase_H__

#include "HNPlatform/HNPlatformNotifyDelegate.h"
#include "HNSocket/HNSocketMessage.h"
#include "HNUIExport.h"

namespace HN {

	class HNPlatformBase : public HNScene, IPlatformMessageDelegate
	{
	public:
		HNPlatformBase(void);
		virtual ~HNPlatformBase(void);

	public:
		virtual void onEnter() override;

		virtual void onExit() override;

	};
};

#endif	//__HN_HNPlatformScene_H__
