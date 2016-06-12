//Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved

#include "HNAliPayModule.h"
#include <string>
#import "HNMarket.h"
#import "json.h"
#import "oc-marco.h"
#define MODULE_NAME "alipay"

namespace HN
{
    create_impl(AliPayModule)
    
    AliPayModule::AliPayModule():Module(MODULE_NAME)
    {
        registerAyncMethod("pay", HN_CALLFUNC_SELECTOR(AliPayModule::Pay));
    }
    
    AliPayModule::~AliPayModule()
    {
        [_impl release];
        _impl = nil;
    }
    
    void AliPayModule::Start(const std::string& appID, const std::string& privateKey,const std::string& handleUrl)
    {
        _impl = [[HNAliPay_IMPL alloc] initWithAppID:stringToNSString(appID)
                                          PrivateKey:stringToNSString(privateKey)
                                            Delegate:this
                                           HandleURL:stringToNSString(handleUrl)];
    }
    
    bool AliPayModule::HandleURL(NSURL *url)
    {
        return [_impl handleURL:url];
    }
    
    std::string AliPayModule::Pay(const std::string &args, CALLBACK_PRAGMA *callback)
    {
        Json::Value JsonValue;
        Json::Reader JsonReader;
        if (JsonReader.parse(args, JsonValue))
        {
            _callback = *callback;
            std::string orderID = JsonValue["orderID"].asString();
            double price = JsonValue["price"].asDouble();
            NSString *description = [NSString stringWithFormat:@"%d金币",JsonValue["number"].asUInt()];
            [_impl pay:stringToNSString(orderID) Price:price Description:description];
        }
        
        return "";
    }

#pragma delegate
    
    void AliPayModule::HandlePayResult(bool bSuccess,int platform)
    {
        NSString *result = [NSString stringWithFormat:@"{\"status\":%d,\"platform\":%d}",bSuccess,platform];
        Market::sharedMarket()->responseChannel(&_callback, result.UTF8String);
    }
    
}