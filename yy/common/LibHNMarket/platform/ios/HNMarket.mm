#import "HNMarket.h"
#import "HNOperator.h"
#import "HNSysModule.h"
#import "HNMarketModule.h"
#import "HNPayManager.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "network/HttpRequest.h"
#import <UIKit/UIKit.h>

#define JSON_RESOVE(obj, key) (obj.HasMember(key) && !obj[key].IsNull())

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
        
        PayManager* pay = PayManager::sharedPayManager();
        mModuleMap.registerObject(pay->getModuleName(), pay);
    }
	
    void Market::registerModule(const std::string& key, Module* module)
    {
        assert(module != nullptr);
        assert(!key.empty());
        
        mModuleMap.registerObject(key, module);
    }
    
	std::string Market::requestChannel(const std::string& module, const std::string& method,
                                       const std::string& args, CALLBACK_PRAGMA* callback)
	{
#ifdef DEBUG
        NSString* log = [NSString stringWithFormat: @"module = %s method = %s, args = %s",
                         module.c_str(), method.c_str(), args.c_str()];
        NSLog(@"%@", log);
#endif
        Module* pModule = mModuleMap.find(module);
        if (pModule != nullptr) {
            return pModule->execute(method, args, callback);
        }
        return "-1";
	}

	void Market::responseChannel(CALLBACK_PRAGMA* callback, const std::string& args)
	{
        Operator::sharedOperator()->responseChannel(callback, args);
	}
    
    void Market::pay(const std::string& args,  const std::string& data, CALLBACK_PRAGMA* callback)
    {
        rapidjson::Document doc;
        doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
        
        if (doc.HasParseError())
        {
            return;
        }
        
        // check code send to app store for server.
        int payType = -1;
        if (doc.IsObject() && doc.HasMember("payType"))
        {
            payType = doc["payType"].GetInt();
        }
        
        switch (payType) {
            case 0:
                requestChannel("wepay", "pay", data, callback);
                break;
            case 1:
                requestChannel("alipay", "pay", data, callback);
                break;
            case 3:
                requestChannel("dinpay", "pay", data, callback);
                break;
            case 4:
                requestChannel("iap", "pay", args, callback);
                break;
            default:
                break;
        }
    }
    
    void Market::payCB(CALLBACK_PRAGMA* callback, const std::string& args)
    {

    }
}
