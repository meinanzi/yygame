//Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved

#include "HNDinPayModule.h"
#import "HNMarket.h"
#import "json.h"
#include "oc-marco.h"
#define MODULE_NAME "dinpay"

namespace HN
{
    create_impl(DinPayModule)
    
    DinPayModule::DinPayModule():Module(MODULE_NAME)
    {
        registerAyncMethod("pay", HN_CALLFUNC_SELECTOR(DinPayModule::Pay));
    }
    
    DinPayModule::~DinPayModule()
    {
        [_impl release];
        _impl = nil;
    }
    
    void DinPayModule::Start(UIViewController *viewController)
    {
        _impl = [[HNDinPay_IMPL alloc] initWithViewController:viewController];
    }
    
    std::string DinPayModule::Pay(const std::string &args, CALLBACK_PRAGMA* callback)
    {
        Json::Value JsonValue;
        Json::Reader JsonReader;
        if (JsonReader.parse(args, JsonValue))
        {
            std::string identifier = JsonValue["identifier"].asString();
            double price = JsonValue["price"].asDouble();
            [_impl pay:stringToNSString(identifier) Price:price CallBackFunc: callback];
        }
        
        return "";
    }
    
#pragma delegate
}