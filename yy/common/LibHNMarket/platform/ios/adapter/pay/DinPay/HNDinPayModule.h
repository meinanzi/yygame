#ifndef LibHNMarket_HNDinPayModule_h
#define LibHNMarket_HNDinPayModule_h

#include "HNModule.h"
#include "HNDinPay_IMPL.h"
#import <UIKit/UIKit.h>

namespace HN
{
    class DinPayModule:public Module
    {
        create_declare(DinPayModule)
        
    public:
        ~DinPayModule();
        
        void Start(UIViewController *viewController);
        
    private:
        DinPayModule();
        
        std::string Pay(const std::string& args, CALLBACK_PRAGMA* callback);
        
    private:
        HNDinPay_IMPL* _impl;
    };
}


#endif
