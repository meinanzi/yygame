#import "HNDinPay_IMPL.h"
#include <UIKit/UIKit.h>
#import "platform/ios/CCEAGLView-ios.h"
#import "cocos2d.h"
#include "HNDinPayModule.h"
#import "HNMarket.h"
#import "DinUIViewController.h"
#import "UIViewController+PotraitViewController.h"

@interface HNDinPay_IMPL()
{
    UIViewController*  _viewController;
    
    DinViewController* _resultViewController;
}
@end

@implementation HNDinPay_IMPL

- (void)dealloc
{
    [super dealloc];
}

- (id)getAppController {
    id controller = [[UIApplication sharedApplication] delegate];
    return controller;
}

- (id)initWithViewController:(UIViewController *)viewController
{
    if (self = [super init]) {
        NSAssert(viewController != nil, @"ViewController = nil");
        _viewController = viewController;
        }
    return self;
}


- (void)pay:(NSString *) productInfo Price:(double)price CallBackFunc: (CALLBACK_PRAGMA*) _callbackFunc{
    
    UIWindow *window;
    
    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    
    // Init the CCEAGLView
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [window bounds]
                                         pixelFormat: (NSString*)cocos2d::GLViewImpl::_pixelFormat
                                         depthFormat: cocos2d::GLViewImpl::_depthFormat
                                  preserveBackbuffer: NO
                                          sharegroup: nil
                                       multiSampling: NO
                                     numberOfSamples: 0 ];
    
    // Enable or disable multiple touches
    [eaglView setMultipleTouchEnabled:NO];
    
    // Use RootViewController manage CCEAGLView
    _resultViewController = [[DinViewController alloc] initWithNibName:nil bundle:nil];
    _resultViewController.wantsFullScreenLayout = YES;
    _resultViewController.view = eaglView;
    
    _resultViewController._callback = *_callbackFunc; // set the callback function
    
    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: _resultViewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:_resultViewController];
    }

    
    Order* ors = [[Order alloc]init];
    ors.merchant = @"2000631543";
    ors.notify_url = @"http://183.234.117.210:9090/return/return.jsp";

    NSDateFormatter* fmt =  [[NSDateFormatter alloc]init];
    [fmt setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    ors.order_time = [fmt stringFromDate:[NSDate date]];
    
    [fmt setDateFormat:@"yyyyMMddHHmmss"]; 
    ors.order_no = [fmt stringFromDate:[NSDate date]];
    
    //ors.oreder_amount = @"0.1";
    ors.oreder_amount = [NSString stringWithFormat:@"%.2f", price];
    ors.interface_version = @"V3.0";
    ors.product_name = @"jinbi";
    ors.sign_type = @"MD5";
    ors.redo_flag = @"1";
    //需要签名的参数排序后组成的字符串
    NSMutableString* signStr = [ors getSortStrFromOrder];
    
    //用于MD5签名的key
    NSString* key = @"2248rerlernhwpemEJK33_FENBZ8jd";
    [signStr appendString:[NSString stringWithFormat:@"&key=%@", key]];
    ors.sign = [Order md5:signStr];
    
    [[self getAppController] setSupportPortrait:true];
    
    [DPPlugin startDinPay:ors mode:@"00" viewController:_viewController delegate:_resultViewController];
    
    //[[self getAppController] setSupportPortrait:false];
}

@end

