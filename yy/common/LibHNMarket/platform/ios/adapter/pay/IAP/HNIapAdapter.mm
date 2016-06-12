#import "HNIapAdapter.h"
#import "IapAdapter_IMPL.h"

#define MODULE_NAME     "iap"

////////////////////////////////////////////////////////////////////////////////////////////////////
namespace HN
{
    create_impl(IapAdapter)
    
    IapAdapter::IapAdapter() : PayModule(MODULE_NAME)
    {
    }
    
    IapAdapter::~IapAdapter()
    {
    }
    
    void IapAdapter::start(UIViewController *viewController, const std::string& appId,
                           const std::string& appKey, PayDelegate* payDelegate)
    {
        [[IapAdapter_IMPL sharedIapAdapter_IMPL] start:viewController
                                                 appId:[NSString stringWithUTF8String:appId.c_str()]
                                                appKey:[NSString stringWithUTF8String:appKey.c_str()]];
        [[IapAdapter_IMPL sharedIapAdapter_IMPL] setDelegate:payDelegate];
    }
    
    void IapAdapter::pay()
    {
        [[IapAdapter_IMPL sharedIapAdapter_IMPL] pay:[NSString stringWithUTF8String:getProductId().c_str()]];
    }
    
    void IapAdapter::restore()
    {
        [[IapAdapter_IMPL sharedIapAdapter_IMPL] restore];
    }
    
    void IapAdapter::addProcuct(NSDictionary* product)
    {
        [IapAdapter_IMPL sharedIapAdapter_IMPL].ProcductDic = product;
    }
    
    
    void IapAdapter::setVerify(bool verify)
    {
        [[IapAdapter_IMPL sharedIapAdapter_IMPL] setVerify:verify];
    }
    
}
