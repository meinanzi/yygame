#ifndef __HNSDK__PayModule__
#define __HNSDK__PayModule__

#import "HNModule.h"
#import "paycb.h"

namespace HN
{
    class PayModule : public Module
    {
    public:
        /**
         *  Description
         *
         *  @param moduleName moduleName description
         *
         *  @return return value description
         */
        PayModule(const std::string& moduleName): Module(moduleName){}
        
    public:
        /**
         *  获取产品id
         *
         *  @return return value description
         */
        const std::string& getProductId() { return mProductId; }
        
        /**
         *  设置产品id
         *
         *  @param productId productId description
         */
        void setProductId(const std::string& productId) {
            this->mProductId = productId;
        }
        
    public:
        /**
         *  Description
         *
         *  @param viewController viewController description
         *  @param appid          appid description
         *  @param appkey         appkey description
         */
        virtual void start(UIViewController *viewController,
                           const std::string& appid, const std::string& appkey, PayDelegate* payDelegate) = 0;
        
        /**
         *  Description
         *
         *  @param block block description
         */
        virtual void pay() = 0;
        
        
        /**
         *  Description
         */
        virtual void restore() {};
        /**
         *  Description
         *
         *  @return return value description
         */
        virtual bool isLogin() { return false; }
        /**
         *  Description
         *
         *  @param block block description
         */
        virtual void login() {};
        /**
         *  Description
         *
         *  @param block block description
         */
        virtual void logout() {};
        
    private:
        std::string     mProductId;
    };
}

#endif      // __HNSDK__PayModule__