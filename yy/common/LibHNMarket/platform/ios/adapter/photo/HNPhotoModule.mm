//Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved



#include "HNPhotoModule.h"
#import "HNMarket.h"
#import "json.h"
#import "oc-marco.h"

#define MODULE_NAME "hnphoto"

namespace HN
{
    
    create_impl(HNPhotoModule)
    
    HNPhotoModule::HNPhotoModule():Module(MODULE_NAME)
    {
        registerAyncMethod("getphoto", HN_CALLFUNC_SELECTOR(HNPhotoModule::GetPhoto));
    }

    HNPhotoModule::~HNPhotoModule()
    {
        [_impl release];
        _impl = nil;
    }
    
    void HNPhotoModule::Start(UIViewController *viewController)
    {
        _impl = [[HNPhotoAdapter_IMPL alloc] initWithViewController:viewController Delegate:this];
    }
    
    std::string HNPhotoModule::GetPhoto(const std::string &args,CALLBACK_PRAGMA* callback)
    {
        Json::Value JsonValue;
        Json::Reader JsonReader;
        if (JsonReader.parse(args, JsonValue))
        {
            _callback = *callback;
            int x = JsonValue["x"].isInt() ? JsonValue["x"].asInt() : 500;
            int y = JsonValue["y"].asInt() ? JsonValue["y"].asInt() : 500;
            [_impl getPhoto:x :y];
        }
        
        
        return "";
    }
    
    void HNPhotoModule::HandleGetPhoto(const std::string &data)
    {
        Market::sharedMarket()->responseChannel(&_callback, data);
    }
}