#include "HNOperator.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	#include "platform/android/HNMarket.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#include "platform/win32/HNMarket.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	#include "platform/ios/HNMarket.h"
#else
	#error  "no support"
#endif

namespace HN
{
	static Operator gsOperator;

	Operator* Operator::sharedOperator()
	{
		return &gsOperator;
	}
	
	Operator::Operator()
	{
	}
	
	std::string Operator::requestChannel(const std::string& module, const std::string& method,
                                         const std::string& args, CALLBACK_PRAGMA* callback)
	{
		std::string ret("");
        ret = Market::sharedMarket()->requestChannel(module, method, args, callback);
		return ret;
	}

    void Operator::responseChannel(CALLBACK_PRAGMA* callback, const std::string& args)
    {
		if (!callback->valid()) return;

		callback->doCallSelector(args);
    }
    
	void Operator::pay(const std::string& identifier, const std::string& data, CALLBACK_PRAGMA* callback)
	{
    	if (!callback->valid()) return;
        
		Market::sharedMarket()->pay(identifier, data, callback);
	}

	void Operator::payCB(CALLBACK_PRAGMA* callback, const std::string& args)
	{
		if (!callback->valid()) return;
		callback->doCallSelector(args);
	}
}
