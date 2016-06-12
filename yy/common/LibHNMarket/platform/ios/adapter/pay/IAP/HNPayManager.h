#ifndef __KooSDK__PayManager__
#define __KooSDK__PayManager__

#import "paycb.h"
#import "HNModule.h"
#import "HNPayCallBack.h"
#import "PayViewController.h"
#include <string>

namespace HN
{
    class PayModule;
    
    class PayManager : public Module, public MultiPayDelegate
                        , public PayDelegate
    {
		create_declare(PayManager)
        
    public:
        void start(UIViewController *viewController, bool MultiPay = false);
        void registerPay(PayModule* payModule,
                         const std::string& appId = "", const std::string& appKey = "");
        void setPayPriority(PayModule* first, const std::string& firstName,
                            PayModule* second, const std::string& secondName);
             
    public:
        virtual void handleMultiPayStart(PayModule* pay) override;
        
        virtual void handleMultiPayCancel() override;
        
    public:
        virtual void handlePayResult(int status, const std::string& ext) override;
        virtual void handleLoginResult(int status) override;
        virtual void handleLogoutResult() override;
        
    private:
        std::string pay(const std::string& args, CALLBACK_PRAGMA* callback);
        std::string restore(const std::string& args, CALLBACK_PRAGMA* callback);
        std::string isLogin(const std::string& args);
        std::string login(const std::string& args, CALLBACK_PRAGMA* callback);
        std::string logout(const std::string& args, CALLBACK_PRAGMA* callback);
        
    private:
        PayManager();
        
    public:
        ~PayManager();
        
    private:
        Controller<PayModule*> _payMap;
        
        PayViewController*     _payViewController;
        
        UIViewController*      _viewController;
        
        bool                   _multiPay;
    };
}

#endif      // __KooSDK__PayManager__