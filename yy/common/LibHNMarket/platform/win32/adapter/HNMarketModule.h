#ifndef __HNMARKETMODULE_H__
#define __HNMARKETMODULE_H__

#include "../../../HNModule.h"
#include "../../../HNPayCallBack.h"

namespace HN
{
    class MarketModule : public Module
    {
		create_declare(MarketModule)

    private:
        // 评价接口
        std::string evaluate(const std::string& args);
		// 评价接口
		std::string webpageRedirect(const std::string& args);
    private:
        MarketModule();
        
    public:
        ~MarketModule();
       
    };
}

#endif	//__HNMARKETMODULE_H__