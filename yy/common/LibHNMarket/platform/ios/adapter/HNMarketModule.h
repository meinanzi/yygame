#ifndef __HNSDK__MarketModule__
#define __HNSDK__MarketModule__

#include "HNModule.h"
#include "HNPayCallBack.h"

namespace HN
{
    class MarketModule : public Module
    {
		create_declare(MarketModule)
      
    private:
        // 评价接口
        std::string evaluate(const std::string& args);
        // 网页跳转接口
        std::string webpageRedirect(const std::string& urlstr);
        
    private:
        MarketModule();
        
    public:
        ~MarketModule();
       
    };
}

#endif      //__HNSDK__MarketModule__