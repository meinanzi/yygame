
#import <Foundation/Foundation.h>
#include "../HNPayDelegate.h"
#import <UIKit/UIKit.h>
#import "DPPlugin.h"
#import "DinPayPluginDelegate.h"
#include "HNModule.h"

@interface HNDinPay_IMPL : NSObject
{
    bool _supportPortrait;
}

//- (void)setSupportPortrait:(bool)support;
- (id)initWithViewController:(UIViewController *)viewController;
- (void)pay:(NSString  *)orderID Price:(double)price CallBackFunc: (CALLBACK_PRAGMA*) _callback;
@end


