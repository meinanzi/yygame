#include "HNMarket.h"
#include "HNOperator.h"
#include "adapter\HNSysModule.h"
#include "adapter\HNMarketModule.h"
#include <algorithm>

namespace HN
{
	create_impl(Market)
    
    Market::Market()
    {
        InitModule();
    }
    
    Market::~Market()
    {
    }
    
    void Market::InitModule()
    {
		SysModule* sys = SysModule::sharedSysModule();
		mModuleMap.registerObject(sys->getModuleName(), sys);

		MarketModule* market = MarketModule::sharedMarketModule();
		mModuleMap.registerObject(market->getModuleName(), market);
    }
	
    void Market::registerModule(const std::string& key, Module* module)
    {
        assert(module != nullptr);
        assert(!key.empty());
        
        mModuleMap.registerObject(key, module);
    }
    
	std::string Market::requestChannel(const std::string& module, const std::string& method,
                                       const std::string& args, CALLBACK_PRAGMA*  callback)
	{
        Module* pModule = mModuleMap.find(module);
        if (pModule != nullptr) {
            return pModule->execute(method, args, callback);
        }
        return "";
	}

	void Market::responseChannel(CALLBACK_PRAGMA*  callback, const std::string& args)
	{
        Operator::sharedOperator()->responseChannel(callback, args);
	}

	void Market::pay(const std::string& identifier, const std::string& data, CALLBACK_PRAGMA* callback)
	{
		registerCB(identifier, callback);
		payCB(identifier, true, "{\"status\":1}");
	}

	void Market::payCB(const std::string& identifier, bool isSucess, const std::string& args)
	{
		CALLBACK_PRAGMA* callback = findCB(identifier);
		
		if (callback && !callback->valid()) return;

		try
		{
			Operator::payCB(callback, args);
		}
		catch (...)
		{
			
		}
		removeCB(identifier);
	}


	void Market::registerCB(const std::string& identifier, CALLBACK_PRAGMA* callback)
	{
		CALLBACK_PRAGMA newCallBack(*callback);
		auto value = std::make_pair(identifier, newCallBack);
		_payQueue.insert(value);
	}

	CALLBACK_PRAGMA* Market::findCB(const std::string& identifier)
	{
		auto iter = _payQueue.find(identifier);
		if (iter != _payQueue.end())
		{
			CALLBACK_PRAGMA* callback = &iter->second;
			return callback;
		}
		else
		{
			return nullptr;
		}
	}

	void Market::removeCB(const std::string& pointID)
	{
		auto iter = _payQueue.find(pointID);
		if (iter != _payQueue.end())
		{
			_payQueue.erase(iter);
		}
	}
}
