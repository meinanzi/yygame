#include "HNPayModule.h"
#include "HNPayCallBack.h"

#ifndef __HNSDK__IapAdapter__
#define __HNSDK__IapAdapter__

namespace HN
{
    class IapAdapter : public PayModule
    {
		create_declare(IapAdapter)
  
    public:
        virtual void start(UIViewController *viewControllerconst, const std::string& appid,
                           const std::string& appkey, PayDelegate* payDelegate) override;
        virtual void pay() override;
        
        virtual void restore() override;
        
    public:
        void setVerify(bool verify);
        void addProcuct(NSDictionary* product);
        
    private:
        std::string pay(const std::string& args, CALLBACK_PRAGMA* callback);
        
        
    private:
        IapAdapter();
        
    public:
        ~IapAdapter();
        
    };
}

#endif  // __HNSDK__IapAdapter__