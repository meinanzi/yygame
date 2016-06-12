/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "AppController.h"
#import "platform/ios/CCEAGLView-ios.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "UMeng/Platforms/iOS/UMSocial_Sdk_4.2/Header/UMSocial.h"

#import "platform/ios/HNMarket.h"
#import "platform/ios/adapter/photo/HNPhotoModule.h"
#import "platform/ios/adapter/photo/oc/UIImagePickerController+LandScapeImagePicker.h"
#import "platform/ios/adapter/pay/AliPay/HNAliPayModule.h"
#import <AlipaySDK/AlipaySDK.h>
#import "platform/ios/adapter/pay/DinPay/HNDinPayModule.h"
#import "platform/ios/adapter/pay/IAP/HNIapAdapter.h"
#import "platform/ios/adapter/pay/IAP/HNPayManager.h"

#define ALIPAY_ID "2088011485506714"
#define PRIVITE_KEY "MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBANBscNHNcRCd8lIWYHfFOj5H0+665FFlZ9pQOXLDEwYqWVC1DAUvM/XDHwKTkDSRreBG0mFURpCVRykrNmwlW+nEnZp/SWS3p059ZzEKLqPDAodG2LUOHMFwxooDGSFiXF/m+kfmwzKwnFZ5z7litUPLcOt38jZnkre+HlILW3Z9AgMBAAECgYBhiO2+8MIhgZidm4UnZ2+bJ6fhByrDgI36FyLbNrjXYMgNQbxPTMinENmZLg8E2sugsePP0k13abQ0atl8AHui8INFGOHF4FqgAAgYRUFxBoCM5VOGPVDCzIVZeEE1G741KozMPGxjxmBa9R/pQ5ypQOt1+k95y/MPFAFlRfPNTQJBAPJcKVt+3Xu6AwWU1e+wJoEzFF8cnxF4yKMyVxqG3d9E0ny/xpMD+eSTqIR+kVGbftC5aTLbE95GSS8bAT/4u2MCQQDcJ1XPUZI1p20ErgSdhblVEACxDp5BkDJJrOPSeH4vVWoIgh4V5bvxEb3lu+FlJi7O/QAFsI4tmxqaIKDsstyfAkEAkNLVu+DxhM/AooMv3AzM1YOqdrNpsk0Ux8B7IFTbHGHJzdXjWVKU7yv1FUd0mulqPFuvKz6Sfs5NSSK/+nQttQJBAKJBB1g1qUmL7KgvTM1/2fjcQ3Y67N4rVTTBjGL5VR6zl+NParb+y0Y/qjJgBx2SR5tcEaEf35mzH6cKdzAcHscCQFJKilmqiIIaVlwABkRZ/SZS1+yBs24KLR93zFiT2bQoUeRxrXzJjjBhOZB2cTuqJSgkmYbHKwhlYVzD0ZzKMe0="
#define ALIPAY_URL "http://yc.kl78.com/manage/pay/alipay/alipay_mobile.aspx"

@implementation AppController

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

- (void)setSupportPortrait:(bool)support {
    _supportPortrait = support;
}

-(void) changeScreen:(bool) changed{
    _supportPortrait = changed;
}

- (NSUInteger)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window
{
    if (_supportPortrait) {
        return UIInterfaceOrientationMaskAllButUpsideDown;
    }
    return UIInterfaceOrientationMaskLandscape;
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation{
    //跳转支付宝钱包进行支付，处理支付结果
    [[AlipaySDK defaultService] processOrderWithPaymentResult:url standbyCallback:^(NSDictionary *resultDic) {
        NSString* resultCode = [resultDic objectForKey:@"resultStatus"];
        bool status = false;
        if ([resultCode isEqualToString:@"9000"])
        {
            status = true;
        }
        
        //NSString *result = [NSString stringWithFormat:@"{\"status\":%d,\"platform\":%d}",status,2];
        //HN::Market::sharedMarket()->responseChannel(&_callback, result.UTF8String);
    }];
    
    //return [UMSocialSnsService handleOpenURL:url];
    return YES;
    
    //return [UMSocialSnsService handleOpenURL:url];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    

	[[UIApplication sharedApplication] setIdleTimerDisabled: YES];
    cocos2d::Application *app = cocos2d::Application::getInstance();
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();

    // Override point for customization after application launch.

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
    _viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    _viewController.wantsFullScreenLayout = YES;
    _viewController.view = eaglView;

    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: _viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:_viewController];
    }

    [window makeKeyAndVisible];

    [[UIApplication sharedApplication] setStatusBarHidden:true];

    // DinPay支付接入
    HN::DinPayModule* dinpay = HN::DinPayModule::sharedDinPayModule();
    dinpay->Start(_viewController);
    HN::Market::sharedMarket()->registerModule(dinpay->getModuleName(), dinpay);
    
    // AliPay支付接入
    HN::AliPayModule* alipay = HN::AliPayModule::sharedAliPayModule();
    alipay->Start(ALIPAY_ID, PRIVITE_KEY, ALIPAY_URL);
    HN::Market::sharedMarket()->registerModule(alipay->getModuleName(), alipay);
    
    // IAP支付接入
    HN::IapAdapter* iap = HN::IapAdapter::sharedIapAdapter();
    HN::PayManager::sharedPayManager()->start(_viewController);
    HN::PayManager::sharedPayManager()->registerPay(iap);

    // IMPORTANT: Setting the GLView should be done after creating the RootViewController
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView(eaglView);
    cocos2d::Director::getInstance()->setOpenGLView(glview);
    
    // 推广员获取相册
    HN::HNPhotoModule* photo = HN::HNPhotoModule::sharedHNPhotoModule();
    photo->Start(_viewController);
    HN::Market::sharedMarket()->registerModule(photo->getModuleName(), photo);

    app->run();

    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->pause(); */
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->resume(); */
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc {
    [window release];
    [super dealloc];
}


@end
