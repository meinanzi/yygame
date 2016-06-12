#import "HNMarketModule.h"
#import "HNUtility.h"

#define IOS7_EVALUATE_URL  @"itms-apps://itunes.apple.com/app/id%@"
#define IOS_EVALUATE_URL   @"itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=%@"

#define MODULE_NAME     "marketmodule"

namespace HN
{
    create_impl(MarketModule);
    
    MarketModule::MarketModule() : Module(MODULE_NAME)
    {
        resisterMethod("evaluate", HN_SELECTOR(MarketModule::evaluate));
        resisterMethod("webpageredirect", HN_SELECTOR(MarketModule::webpageRedirect));
    }
    
    MarketModule::~MarketModule()
    {
    }
    
    std::string MarketModule::evaluate(const std::string& args)
    {
        float systemVersion = [[UIDevice currentDevice].systemVersion floatValue];
        NSString* strUrl = systemVersion >= 7.0f ? IOS7_EVALUATE_URL : IOS_EVALUATE_URL;
        NSString* AppID = [HNUtility getAppID];
        NSString *url = [NSString stringWithFormat:strUrl, AppID];
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
        return "";
    }
    
    std::string MarketModule::webpageRedirect(const std::string &urlstr)
    {
        assert(!urlstr.empty());
        if (urlstr.empty()) {
            return "";
        }
        NSURL *url = [NSURL URLWithString:[NSString stringWithUTF8String:urlstr.c_str()]];
        [[UIApplication sharedApplication] openURL:url];
        return "";
    }
}