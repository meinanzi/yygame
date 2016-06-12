#include "HNMarketModule.h"

#define MODULE_NAME     "marketmodule"

namespace HN
{
    create_impl(MarketModule);
    
    MarketModule::MarketModule() : Module(MODULE_NAME)
    {
		resisterMethod("evaluate", HN_SELECTOR(MarketModule::evaluate));
		resisterMethod("webpageRedirect", HN_SELECTOR(MarketModule::webpageRedirect));
    }
    
    MarketModule::~MarketModule()
    {
    }
    
    std::string MarketModule::evaluate(const std::string& args)
    {
        return "";
    }

	std::string MarketModule::webpageRedirect(const std::string& args)
	{
		return "";
	}
}