//Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved

#ifndef __HNHNPhotoModule__
#define __HNHNPhotoModule__

#include "HNModule.h"
#import "oc/HNPhotoAdapter_IMPL.h"
#import <UIKit/UIKit.h>

namespace HN
{
    class HNPhotoModule:public Module,public HNPhotoDelegate
    {
        create_declare(HNPhotoModule)
        
    public:
        ~HNPhotoModule();
        
        void Start(UIViewController *viewController);
        
    private:
        HNPhotoModule();
        
        std::string GetPhoto(const std::string &args,CALLBACK_PRAGMA* callback);
        
        virtual void HandleGetPhoto(const std::string& data) override;
        
    private:
        HNPhotoAdapter_IMPL *_impl;
        
        CALLBACK_PRAGMA _callback;
    };
}

#endif //__HNHNPhotoModule__
