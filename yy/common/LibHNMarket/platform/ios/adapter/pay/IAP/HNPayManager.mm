#import "HNPayManager.h"
#import "HNMarket.h"
#import "HNPayModule.h"
#import "../../../external/json/json.h"

#define MODULE_NAME         "iap"

////////////////////////////////////////////////////////////////////////////////
namespace HN
{
    CALLBACK_PRAGMA gPayCallback;
    CALLBACK_PRAGMA gLoginCallback;
    CALLBACK_PRAGMA gLogoutCallback;
    
    create_impl(PayManager)
    
    PayManager::PayManager()
        : Module(MODULE_NAME)
        , _multiPay(false)
        , _viewController(nil)
        , _payViewController(nil)
    {
        registerAyncMethod("pay", HN_CALLFUNC_SELECTOR(PayManager::pay));
        registerAyncMethod("login", HN_CALLFUNC_SELECTOR(PayManager::login));
        registerAyncMethod("logout", HN_CALLFUNC_SELECTOR(PayManager::logout));
        registerAyncMethod("restore", HN_CALLFUNC_SELECTOR(PayManager::restore));
        
        resisterMethod("islogin", HN_SELECTOR(PayManager::isLogin));
    }
    
    PayManager::~PayManager()
    {
        if (this->_multiPay)
        {
            [_payViewController release];
            _payViewController = nil;
        }
    }
    
    void PayManager::start(UIViewController *viewController, bool MultiPay)
    {
        assert(viewController != nil);
        if (viewController == nil) return;
        
        _viewController = viewController;
        
        this->_multiPay = MultiPay;
        
        if (this->_multiPay && _payViewController == nil)
        {
            _payViewController = [[PayViewController alloc] init];
            [_payViewController start:_viewController MultiPayDelegate:this];
        }
        else
        {
            [_payViewController release];
            _payViewController = nil;
        }
    }
    
    void PayManager::registerPay(PayModule* payModule, const std::string& appId,
                                const std::string& appKey)
    {
        assert(payModule != nullptr);
        if (payModule == nullptr) return;
        
        assert(_viewController != nullptr);
        if (_viewController == nullptr) return;

        payModule->start(_viewController, appId, appKey, this);
        
        _payMap.registerObject(payModule->getModuleName(), payModule);
    }

    void PayManager::setPayPriority(PayModule* first, const std::string& firstName,
                                   PayModule* second, const std::string& secondName)
    {
        assert(_payViewController != nullptr);
        if (_payViewController == nullptr) return;

        [_payViewController setPayPriority:first firstName:[NSString stringWithUTF8String:firstName.c_str()]
                                    second:second secondName:[NSString stringWithUTF8String:secondName.c_str()]];
    }
    
    std::string PayManager::pay(const std::string& args, CALLBACK_PRAGMA* callback)
    {
        // 参数校验
        assert(!args.empty());
        if (args.empty()) return "-1";
        
        // 参数校验
        assert(nullptr != callback);
        if (!callback->valid()) return "-1";
        
        gPayCallback = *callback;
        
        // 更新产品ID
        Controller<PayModule*>::ITERATOR iter =_payMap.begin();
        for (; iter != _payMap.end(); ++iter) {
            iter->second->setProductId(args);
        }
        
        // 多计费模式
        if (this->_multiPay)
        {
            assert(_payViewController != nil);
            if (_payViewController == nil) return "-1";
            
            [_payViewController showPay];
        }
        else
        {
            Controller<PayModule*>::ITERATOR iter = _payMap.begin();
            for (; iter != _payMap.end(); ++iter)
            {
                iter->second->pay();
            }
        }
        
        return "";
    }
    
    std::string PayManager::restore(const std::string& args, CALLBACK_PRAGMA*  callback)
    {
        if (!callback->valid()) return "-1";
        
        gPayCallback = *callback;
        Controller<PayModule*>::ITERATOR iter = _payMap.begin();
        for (; iter != _payMap.end(); ++iter)
        {
            iter->second->restore();
        }
        
        return "";
    }
    
    std::string PayManager::isLogin(const std::string& args)
    {
        Controller<PayModule*>::ITERATOR iter = _payMap.begin();
        for (; iter != _payMap.end(); ++iter)
        {
            bool login = iter->second->isLogin();
            return login ? "1" : "0";
        }
        return "0";
    }
    
    std::string PayManager::login(const std::string& args, CALLBACK_PRAGMA* callback)
    {
        if (!callback->valid()) return "-1";
        
        gLoginCallback = *callback;
        Controller<PayModule*>::ITERATOR iter = _payMap.begin();
        for (; iter != _payMap.end(); ++iter)
        {
            iter->second->login();
        }
        return "0";
    }
    
    std::string PayManager::logout(const std::string& args, CALLBACK_PRAGMA* callback)
    {
        if (!callback->valid()) return "-1";
        
        gLogoutCallback = *callback;
        Controller<PayModule*>::ITERATOR iter = _payMap.begin();
        for (; iter != _payMap.end(); ++iter)
        {
            iter->second->logout();
        }
        return "0";
    }
    
    void PayManager::handleMultiPayStart(PayModule* pay)
    {
        pay->pay();
    }
    
    void PayManager::handleMultiPayCancel()
    {
        Json::Value JsonRoot;
        JsonRoot["status"] = false;
        JsonRoot["ext"] = "支付取消";
        Json::FastWriter Writer;
        Market::sharedMarket()->responseChannel(&gPayCallback, Writer.write(JsonRoot));
    }
    
    void PayManager::handlePayResult(int status, const std::string& ext)
    {
        Json::Value JsonRoot;
        JsonRoot["status"] = status;
        JsonRoot["ext"]    = ext;
        Json::FastWriter Writer;
        Market::sharedMarket()->responseChannel(&gPayCallback, Writer.write(JsonRoot));
    }
    
    void PayManager::handleLoginResult(int status)
    {
        Json::Value JsonRoot;
        JsonRoot["status"] = status;
        Json::FastWriter Writer;
        Market::sharedMarket()->responseChannel(&gLoginCallback, Writer.write(JsonRoot));
    }
    
    void PayManager::handleLogoutResult()
    {
        Market::sharedMarket()->responseChannel(&gLogoutCallback, "");
    }
}