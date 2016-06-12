//Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved

#ifndef __HNAliPayModule__
#define __HNAliPayModule__

#include "HNModule.h"
#include "HNAliPay_IMPL.h"
#include "../HNPayDelegate.h"
namespace HN
{
    class AliPayModule:public Module ,public HNPayDelegate
    {
        create_declare(AliPayModule)
        
    public:
        ~AliPayModule();
        
        void Start(const std::string& appID,const std::string& privateKey,const std::string& handleUrl);
        
        bool HandleURL(NSURL *url);
        
    private:
        AliPayModule();
        
        std::string Pay(const std::string &args,CALLBACK_PRAGMA* callback);
        
        void HandlePayResult(bool bSuccess,int platform) override;
        
    private:
        HNAliPay_IMPL* _impl;
        CALLBACK_PRAGMA _callback;
    };
}

#endif //__HNAliPayModule__
